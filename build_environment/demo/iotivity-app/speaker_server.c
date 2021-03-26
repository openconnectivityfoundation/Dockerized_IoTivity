/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright 2017-2019 Open Connectivity Foundation
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

/* Application Design
*
* support functions:
* app_init
*  initializes the oic/p and oic/d values.
* register_resources
*  function that registers all endpoints, e.g. sets the RETRIEVE/UPDATE handlers for each end point
*
* main
*  starts the stack, with the registered resources.
*
* Each resource has:
*  global property variables (per resource path) for:
*    the property name
*       naming convention: g_<path>_RESOURCE_PROPERTY_NAME_<propertyname>
*    the actual value of the property, which is typed from the json data type
*      naming convention: g_<path>_<propertyname>
*  global resource variables (per path) for:
*    the path in a variable:
*      naming convention: g_<path>_RESOURCE_ENDPOINT
*    array of interfaces, where by the first will be set as default interface
*      naming convention g_<path>_RESOURCE_INTERFACE
*
*  handlers for the implemented methods (get/post)
*   get_<path>
*     function that is being called when a RETRIEVE is called on <path>
*     set the global variables in the output
*   post_<path>
*     function that is being called when a UPDATE is called on <path>
*     checks the input data
*     if input data is correct
*       updates the global variables
*
*/
/*
 tool_version          : 20200103
 input_file            : ./output/out_codegeneration_merged.swagger.json
 version of input_file :
 title of input_file   : speaker_server
*/

#include "oc_api.h"
#include "port/oc_clock.h"
#include <signal.h>

#ifdef __linux__
/* linux specific code */
#include <pthread.h>
#ifndef NO_MAIN
static pthread_mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
#endif /* NO_MAIN */
#endif

#ifdef WIN32
/* windows specific code */
#include <windows.h>
static CONDITION_VARIABLE cv;   /* event loop variable */
static CRITICAL_SECTION cs;     /* event loop variable */
#endif

#define btoa(x) ((x)?"true":"false")

#define MAX_STRING 30           /* max size of the strings. */
#define MAX_PAYLOAD_STRING 65   /* max size strings in the payload */
#define MAX_ARRAY 10            /* max size of the array */
/* Note: Magic numbers are derived from the resource definition, either from the example or the definition.*/

volatile int quit = 0;          /* stop variable, used by handle_signal */


/* global property variables for path: "/speaker" */
static char g_speaker_RESOURCE_PROPERTY_NAME_mute[] = "mute"; /* the name for the attribute */
bool g_speaker_mute = false; /* current value of property "mute" The mute setting of an audio rendering device. */
static char g_speaker_RESOURCE_PROPERTY_NAME_range[] = "range"; /* the name for the attribute */

/* array range  The valid range for the Property in the Resource as an integer. The first value in the array is the minimum value, the second value in the array is the maximum value. */
int g_speaker_range[2];
size_t g_speaker_range_array_size;

static char g_speaker_RESOURCE_PROPERTY_NAME_step[] = "step"; /* the name for the attribute */
int g_speaker_step = 0; /* current value of property "step" Step value across the defined range when the range is an integer.  This is the increment for valid values across the range; so if range is 0..10 and step is 2 then valid values are 0,2,4,6,8,10. */
static char g_speaker_RESOURCE_PROPERTY_NAME_volume[] = "volume"; /* the name for the attribute */
int g_speaker_volume = 50; /* current value of property "volume" The volume setting of an audio rendering device. *//* registration data variables for the resources */

/* global resource variables for path: /speaker */
static char g_speaker_RESOURCE_ENDPOINT[] = "/speaker"; /* used path for this resource */
static char g_speaker_RESOURCE_TYPE[][MAX_STRING] = {"oic.r.audio"}; /* rt value (as an array) */
int g_speaker_nr_resource_types = 1;
static char g_speaker_RESOURCE_INTERFACE[][MAX_STRING] = {"oic.if.a","oic.if.baseline"}; /* interface if (as an array) */
int g_speaker_nr_resource_interfaces = 2;
/**
* function to set up the device.
*
*/
static int
app_init(void)
{
  int ret = oc_init_platform("ocf", NULL, NULL);
  /* the settings determine the appearance of the device on the network
     can be OCF1.3.1 or OCF2.0.0 (or even higher)
     supplied values are for OCF1.3.1 */
  ret |= oc_add_device("/oic/d", "None", "speaker_server",
                       "ocf.2.0.5", /* icv value */
                       "ocf.res.1.3.0, ocf.sh.1.3.0",  /* dmv value */
                       NULL, NULL);
  return ret;
}

/**
* helper function to convert the interface string definition to the constant defintion used by the stack.
* @param interface the interface string e.g. "oic.if.a"
* @return the stack constant for the interface
*/
static int
convert_if_string(char *interface_name)
{
  if (strcmp(interface_name, "oic.if.baseline") == 0) return OC_IF_BASELINE;  /* baseline interface */
  if (strcmp(interface_name, "oic.if.rw") == 0) return OC_IF_RW;              /* read write interface */
  if (strcmp(interface_name, "oic.if.r" )== 0) return OC_IF_R;                /* read interface */
  if (strcmp(interface_name, "oic.if.s") == 0) return OC_IF_S;                /* sensor interface */
  if (strcmp(interface_name, "oic.if.a") == 0) return OC_IF_A;                /* actuator interface */
  if (strcmp(interface_name, "oic.if.b") == 0) return OC_IF_B;                /* batch interface */
  if (strcmp(interface_name, "oic.if.ll") == 0) return OC_IF_LL;              /* linked list interface */
  return OC_IF_A;
}

/**
* helper function to check if the POST input document contains
* the common readOnly properties or the resouce readOnly properties
* @param name the name of the property
* @return the error_status, e.g. if error_status is true, then the input document contains something illegal
*/
static bool
check_on_readonly_common_resource_properties(oc_string_t name, bool error_state)
{
  if (strcmp ( oc_string(name), "n") == 0) {
    error_state = true;
    PRINT ("   property \"n\" is ReadOnly \n");
  }else if (strcmp ( oc_string(name), "if") == 0) {
    error_state = true;
    PRINT ("   property \"if\" is ReadOnly \n");
  } else if (strcmp ( oc_string(name), "rt") == 0) {
    error_state = true;
    PRINT ("   property \"rt\" is ReadOnly \n");
  } else if (strcmp ( oc_string(name), "id") == 0) {
    error_state = true;
    PRINT ("   property \"id\" is ReadOnly \n");
  } else if (strcmp ( oc_string(name), "id") == 0) {
    error_state = true;
    PRINT ("   property \"id\" is ReadOnly \n");
  }
  return error_state;
}



/**
* get method for "/speaker" resource.
* function is called to intialize the return values of the GET method.
* initialisation of the returned values are done from the global property values.
* Resource Description:
* This Resource defines basic audio control functions.
* The Property "volume" is an integer containing a percentage [0,100].
* A volume of 0 (zero) means no sound produced.
* A volume of 100 means maximum sound production.
* The Property "mute" is implemented as a boolean.
* A mute value of true means that the device is muted (no audio).
* A mute value of false means that the device is not muted (audio).
*
* @param request the request representation.
* @param interfaces the interface used for this call
* @param user_data the user data.
*/
static void
get_speaker(oc_request_t *request, oc_interface_mask_t interfaces, void *user_data)
{
  (void)user_data;  /* variable not used */
  /* TODO: SENSOR add here the code to talk to the HW if one implements a sensor.
     the call to the HW needs to fill in the global variable before it returns to this function here.
     alternative is to have a callback from the hardware that sets the global variables.

     The implementation always return everything that belongs to the resource.
     this implementation is not optimal, but is functionally correct and will pass CTT1.2.2 */
  bool error_state = false;


  PRINT("-- Begin get_speaker: interface %d\n", interfaces);
  oc_rep_start_root_object();
  switch (interfaces) {
  case OC_IF_BASELINE:
    /* fall through */
  case OC_IF_A:
  PRINT("   Adding Baseline info\n" );
    oc_process_baseline_interface(request->resource);

    /* property (boolean) 'mute' */
    oc_rep_set_boolean(root, mute, g_speaker_mute);
    PRINT("   %s : %s\n", g_speaker_RESOURCE_PROPERTY_NAME_mute,  btoa(g_speaker_mute));

    /* property (array of integers) 'range' */
    oc_rep_set_array(root, range);
    PRINT("   %s int = [ ", g_speaker_RESOURCE_PROPERTY_NAME_range);
    for (int i=0; i< (int)g_speaker_range_array_size; i++) {
      oc_rep_add_int(range, g_speaker_range[i]);
      PRINT("   %d ", g_speaker_range[i]);
    }
    oc_rep_close_array(root, range);
    /* property (integer) 'step' */
    oc_rep_set_int(root, step, g_speaker_step);
    PRINT("   %s : %d\n", g_speaker_RESOURCE_PROPERTY_NAME_step, g_speaker_step);
    /* property (integer) 'volume' */
    oc_rep_set_int(root, volume, g_speaker_volume);
    PRINT("   %s : %d\n", g_speaker_RESOURCE_PROPERTY_NAME_volume, g_speaker_volume);
    break;
  default:
    break;
  }
  oc_rep_end_root_object();
  if (error_state == false) {
    oc_send_response(request, OC_STATUS_OK);
  }
  else {
    oc_send_response(request, OC_STATUS_BAD_OPTION);
  }
  PRINT("-- End get_speaker\n");
}

/**
* post method for "/speaker" resource.
* The function has as input the request body, which are the input values of the POST method.
* The input values (as a set) are checked if all supplied values are correct.
* If the input values are correct, they will be assigned to the global  property values.
* Resource Description:

*
* @param request the request representation.
* @param interfaces the used interfaces during the request.
* @param user_data the supplied user data.
*/
static void
post_speaker(oc_request_t *request, oc_interface_mask_t interfaces, void *user_data)
{
  (void)interfaces;
  (void)user_data;
  bool error_state = false;
  PRINT("-- Begin post_speaker:\n");
  oc_rep_t *rep = request->request_payload;

  /* loop over the request document for each required input field to check if all required input fields are present */
  /* loop over the request document to check if all inputs are ok */
  rep = request->request_payload;
  while (rep != NULL) {
    PRINT("key: (check) %s \n", oc_string(rep->name));

    error_state = check_on_readonly_common_resource_properties(rep->name, error_state);
    if (strcmp ( oc_string(rep->name), g_speaker_RESOURCE_PROPERTY_NAME_mute) == 0) {
      /* property "mute" of type boolean exist in payload */
      if (rep->type != OC_REP_BOOL) {
        error_state = true;
        PRINT ("   property 'mute' is not of type bool %d \n", rep->type);
      }
    }
    if (strcmp ( oc_string(rep->name), g_speaker_RESOURCE_PROPERTY_NAME_volume) == 0) {
      /* property "volume" of type integer exist in payload */
      if (rep->type != OC_REP_INT) {
        error_state = true;
        PRINT ("   property 'volume' is not of type int %d \n", rep->type);
      }

      int value_max = (int) rep->value.integer;
      if ( value_max > 100 ) {
        /* check the maximum range */
        PRINT ("   property 'volume' value exceed max : 0 >  value: %d \n", value_max);
        error_state = true;
      }
    }rep = rep->next;
  }
  /* if the input is ok, then process the input document and assign the global variables */
  if (error_state == false)
  {
    /* loop over all the properties in the input document */
    oc_rep_t *rep = request->request_payload;
    while (rep != NULL) {
      PRINT("key: (assign) %s \n", oc_string(rep->name));
      /* no error: assign the variables */

      if (strcmp ( oc_string(rep->name), g_speaker_RESOURCE_PROPERTY_NAME_mute)== 0) {
        /* assign "mute" */
        PRINT ("  property 'mute' : %s\n", btoa(rep->value.boolean));
        g_speaker_mute = rep->value.boolean;
      }
      if (strcmp ( oc_string(rep->name), g_speaker_RESOURCE_PROPERTY_NAME_volume) == 0) {
        /* assign "volume" */
        PRINT ("  property 'volume' : %d\n", (int) rep->value.integer);
        g_speaker_volume = (int) rep->value.integer;
      }
      rep = rep->next;
    }
    /* set the response */
    PRINT("Set response \n");
    oc_rep_start_root_object();
    /*oc_process_baseline_interface(request->resource); */
    oc_rep_set_boolean(root, mute, g_speaker_mute);

    oc_rep_set_array(root, range);
    for (int i=0; i< (int)g_speaker_range_array_size; i++) {
      oc_rep_add_int(range, g_speaker_range[i]);
    }
    oc_rep_close_array(root, range);

    oc_rep_set_int(root, step, g_speaker_step );
    oc_rep_set_int(root, volume, g_speaker_volume );

    oc_rep_end_root_object();
    /* TODO: ACTUATOR add here the code to talk to the HW if one implements an actuator.
       one can use the global variables as input to those calls
       the global values have been updated already with the data from the request */
    oc_send_response(request, OC_STATUS_CHANGED);
  }
  else
  {
    PRINT("  Returning Error \n");
    /* TODO: add error response, if any */
    //oc_send_response(request, OC_STATUS_NOT_MODIFIED);
    oc_send_response(request, OC_STATUS_BAD_REQUEST);
  }
  PRINT("-- End post_speaker\n");
}
/**
* register all the resources to the stack
* this function registers all application level resources:
* - each resource path is bind to a specific function for the supported methods (GET, POST, PUT)
* - each resource is
*   - secure
*   - observable
*   - discoverable
*   - used interfaces (from the global variables).
*/
static void
register_resources(void)
{

  PRINT("Register Resource with local path \"/speaker\"\n");
  oc_resource_t *res_speaker = oc_new_resource(NULL, g_speaker_RESOURCE_ENDPOINT, g_speaker_nr_resource_types, 0);
  PRINT("     number of Resource Types: %d\n", g_speaker_nr_resource_types);
  for( int a = 0; a < g_speaker_nr_resource_types; a++ ) {
    PRINT("     Resource Type: \"%s\"\n", g_speaker_RESOURCE_TYPE[a]);
    oc_resource_bind_resource_type(res_speaker,g_speaker_RESOURCE_TYPE[a]);
  }
  for( int a = 0; a < g_speaker_nr_resource_interfaces; a++ ) {
    oc_resource_bind_resource_interface(res_speaker, convert_if_string(g_speaker_RESOURCE_INTERFACE[a]));
  }
  oc_resource_set_default_interface(res_speaker, convert_if_string(g_speaker_RESOURCE_INTERFACE[0]));
  PRINT("     Default OCF Interface: \"%s\"\n", g_speaker_RESOURCE_INTERFACE[0]);
  oc_resource_set_discoverable(res_speaker, true);
  /* periodic observable
     to be used when one wants to send an event per time slice
     period is 1 second */
  oc_resource_set_periodic_observable(res_speaker, 1);
  /* set observable
     events are send when oc_notify_observers(oc_resource_t *resource) is called.
    this function must be called when the value changes, perferable on an interrupt when something is read from the hardware. */
  /*oc_resource_set_observable(res_speaker, true); */

  oc_resource_set_request_handler(res_speaker, OC_GET, get_speaker, NULL);

  oc_resource_set_request_handler(res_speaker, OC_POST, post_speaker, NULL);
  oc_add_resource(res_speaker);
}


#ifdef OC_SECURITY
void
random_pin_cb(const unsigned char *pin, size_t pin_len, void *data)
{
  (void)data;
  PRINT("\n====================\n");
  PRINT("Random PIN: %.*s\n", (int)pin_len, pin);
  PRINT("====================\n");
}
#endif /* OC_SECURITY */

void
factory_presets_cb(size_t device, void *data)
{
  (void)device;
  (void)data;
  PRINT("No PKI certificates installed\n");
}


/**
* intializes the global variables
* registers and starts the handler

*/
void
initialize_variables(void)
{
  /* initialize global variables for resource "/speaker" */  g_speaker_mute = false; /* current value of property "mute" The mute setting of an audio rendering device. */
  /* initialize array "range" : The valid range for the Property in the Resource as an integer. The first value in the array is the minimum value, the second value in the array is the maximum value. */g_speaker_range_array_size = 0;
  /* no values found for range, adding default (integer) range */
  g_speaker_range[0] = 0;
  g_speaker_range[1] = 100;
  g_speaker_range_array_size = 2;
  g_speaker_step = 0; /* current value of property "step" Step value across the defined range when the range is an integer.  This is the increment for valid values across the range; so if range is 0..10 and step is 2 then valid values are 0,2,4,6,8,10. */
  g_speaker_volume = 50; /* current value of property "volume" The volume setting of an audio rendering device. */

  /* set the flag for NO oic/con resource. */
  oc_set_con_res_announced(false);

}

#ifndef NO_MAIN

#ifdef WIN32
/**
* signal the event loop (windows version)
* wakes up the main function to handle the next callback
*/
static void
signal_event_loop(void)
{
  WakeConditionVariable(&cv);
}
#endif /* WIN32 */

#ifdef __linux__
/**
* signal the event loop (Linux)
* wakes up the main function to handle the next callback
*/
static void
signal_event_loop(void)
{
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
}
#endif /* __linux__ */

/**
* handle Ctrl-C
* @param signal the captured signal
*/
void
handle_signal(int signal)
{
  (void)signal;
  signal_event_loop();
  quit = 1;
}


/**
* main application.
* intializes the global variables
* registers and starts the handler
* handles (in a loop) the next event.
* shuts down the stack
*/
int
main(void)
{
int init;
  oc_clock_time_t next_event;

#ifdef WIN32
  /* windows specific */
  InitializeCriticalSection(&cs);
  InitializeConditionVariable(&cv);
  /* install Ctrl-C */
  signal(SIGINT, handle_signal);
#endif
#ifdef __linux__
  /* linux specific */
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  /* install Ctrl-C */
  sigaction(SIGINT, &sa, NULL);
#endif

  PRINT("Used input file : \"./output/out_codegeneration_merged.swagger.json\"\n");
  PRINT("OCF Server name : \"speaker_server\"\n");

  /*intialize the variables */
  initialize_variables();


  /* initializes the handlers structure */
  static const oc_handler_t handler = {.init = app_init,
                                       .signal_event_loop = signal_event_loop,
                                       .register_resources = register_resources
#ifdef OC_CLIENT
                                       ,
                                       .requests_entry = 0
#endif
                                       };

#ifdef OC_SECURITY
  PRINT("Intialize Secure Resources\n");
  oc_storage_config("./devicebuilderserver_creds");
#endif /* OC_SECURITY */

#ifdef OC_SECURITY
  /* please comment out if the server:
    - have no display capabilities to display the PIN value
    - server does not require to implement RANDOM PIN (oic.sec.doxm.rdp) onboarding mechanism
  */
  oc_set_random_pin_callback(random_pin_cb, NULL);
#endif /* OC_SECURITY */

  oc_set_factory_presets_cb(factory_presets_cb, NULL);


  /* start the stack */
  init = oc_main_init(&handler);

  if (init < 0) {
    PRINT("oc_main_init failed %d, exiting.\n", init);
    return init;
  }

  PRINT("OCF server \"speaker_server\" running, waiting on incoming connections.\n");

#ifdef WIN32
  /* windows specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    if (next_event == 0) {
      SleepConditionVariableCS(&cv, &cs, INFINITE);
    } else {
      oc_clock_time_t now = oc_clock_time();
      if (now < next_event) {
        SleepConditionVariableCS(&cv, &cs,
                                 (DWORD)((next_event-now) * 1000 / OC_CLOCK_SECOND));
      }
    }
  }
#endif

#ifdef __linux__
  /* linux specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    pthread_mutex_lock(&mutex);
    if (next_event == 0) {
      pthread_cond_wait(&cv, &mutex);
    } else {
      ts.tv_sec = (next_event / OC_CLOCK_SECOND);
      ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
      pthread_cond_timedwait(&cv, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
  }
#endif

  /* shut down the stack */
  oc_main_shutdown();
  return 0;
}
#endif /* NO_MAIN */
