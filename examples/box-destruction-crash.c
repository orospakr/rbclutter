#include <clutter/clutter.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/* gcc `pkg-config --cflags clutter-1.0` `pkg-config --libs clutter-1.0` -o box-destruction-crash box-destruction-crash.c */

ClutterActor *box;

gboolean destroy_timeout(gpointer user_data) {
  printf("Attempting to destroy things!\n");
  clutter_actor_destroy (box);
  return FALSE;
}

int main (int argc, char *argv[]) {
  GError *error = NULL;
  ClutterActor *stage;
  ClutterLayoutManager *layout;
  
  clutter_init_with_args (&argc, &argv,
                          NULL,
                          NULL,
                          NULL,
                          &error);
  if(error) {
    g_warning("Unable to initialise Clutter:\n%s",
              error->message);
    g_error_free (error);
    
    return EXIT_FAILURE;
  }
  
  stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 800, 600);

  // I only crash if I use a bin layout...!?
  layout = g_object_ref_sink (clutter_bin_layout_new (CLUTTER_BIN_ALIGNMENT_CENTER,
                                                      CLUTTER_BIN_ALIGNMENT_CENTER));

  box = clutter_box_new (layout);
  
  clutter_actor_show_all (stage);

  g_timeout_add (3000, destroy_timeout, NULL);

  /* I'm not using the layout itself again (although the Box will
     continue to reference it, for now). */
  g_object_unref (layout);

  clutter_main();
}
