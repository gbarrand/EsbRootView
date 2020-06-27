Module['app_initialize']  = Module.cwrap('app_initialize','number',['string','number', 'number']);
Module['app_render']      = Module.cwrap('app_render','void',['void']);
Module['app_button_down'] = Module.cwrap('app_button_down','void',['number','number','number']);
Module['app_button_up']   = Module.cwrap('app_button_up','void',['number','number','number']);
Module['app_button_move'] = Module.cwrap('app_button_move','void',['number','number','number']);

Module['app_have_to_enable_anim']  = Module.cwrap('app_have_to_enable_anim' ,'number',['void']);
Module['app_reset_have_to_enable_anim']  = Module.cwrap('app_reset_have_to_enable_anim' ,'void',['void']);

Module['app_have_to_disable_anim'] = Module.cwrap('app_have_to_disable_anim','number',['void']);
Module['app_reset_have_to_disable_anim'] = Module.cwrap('app_reset_have_to_disable_anim','void',['void']);

Module['app_load_file']         = Module.cwrap('app_load_file','void',['number','number']);
Module['app_get_doc_file_size'] = Module.cwrap('app_get_doc_file_size','number',['void']);
Module['app_get_doc_file']      = Module.cwrap('app_get_doc_file','number',['void']);
Module['app_delete_buffer']     = Module.cwrap('app_delete_buffer','void',['number']);
