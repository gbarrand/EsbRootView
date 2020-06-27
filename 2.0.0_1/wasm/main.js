const app_main = {};
(function (app_main) {
  'use strict';

  var g_timer_id = null;
  var g_mouse_down = 0;   
    
  function Program(container, canvas) {
    const init = Module.app_initialize("EsbRootView",canvas.width, canvas.height);
    canvas.addEventListener('mousedown', this._handle_button_down.bind(this));
    canvas.addEventListener('mouseup',   this._handle_button_up.bind(this));
    canvas.addEventListener('mousemove', this._handle_button_move.bind(this));
    document.addEventListener('dragover', this._handle_drag_over.bind(this));
    document.addEventListener('drop', this._handle_drop.bind(this));
    this._invalidate();

    var button = document.createElement("button");
    button.innerHTML = "Download app out.png";
    var body = document.getElementsByTagName("body")[0];
    body.appendChild(button);
    button.addEventListener ("click", this._download_out_png.bind(this));      
    button.style.width = '400px';
    button.style.height = '50px';
    button.style.background = 'white';
    button.style.color = 'black';
    button.style.fontSize = '20px';
    button.style.borderWidth = '2px';
    button.style.borderColor = 'black';
    button.style.boxShadow = 'none';
  };

  Program.prototype._invalidate = function () {
    //console.log('debug : _invalidate : begin');
    window.requestAnimationFrame(() => Module.app_render());
    if(Module.app_have_to_enable_anim()==1) {
      Module.app_reset_have_to_enable_anim();
      g_timer_id = setInterval(this._invalidate,16);  // 1/60 seconds.
      //console.log('debug : start timer');
    }      
    if(Module.app_have_to_disable_anim()==1) {
      Module.app_reset_have_to_disable_anim();
      //if(typeof g_idle_timer != 'undefined' ) clearInterval(g_idle_timer);
      clearInterval(g_timer_id);
    }      
  }

  Program.prototype._handle_button_down = function(event) {
    if(event.button==0) {
      g_mouse_down = 1;
      event.stopPropagation();
      event.preventDefault();
      Module.app_button_down(event.button,event.offsetX,event.offsetY);       
      this._invalidate();
    }
  }
  Program.prototype._handle_button_up = function(event) {
    if(event.button==0) {
      g_mouse_down = 0;
      event.stopPropagation();
      event.preventDefault();
      Module.app_button_up(event.button,event.offsetX,event.offsetY);
      this._invalidate();
    }
  }
  Program.prototype._handle_button_move = function(event) {
    if( (event.button==0) && (g_mouse_down==1)) {
      event.stopPropagation();
      event.preventDefault();
      Module.app_button_move(event.button,event.offsetX,event.offsetY);       
      this._invalidate();
    }
  }

  Program.prototype._handle_drag_over = function (event) {
    event.stopPropagation();
    event.preventDefault();
    event.dataTransfer.dropEffect = 'copy';
  }

  Program.prototype._handle_drop = function (event) {
    event.stopPropagation();
    event.preventDefault();
    const files = event.dataTransfer.files;
    if (files && (files.length === 1)) {
      const file_reader = new FileReader();
      file_reader.onload = (event) => {
        this._load_file(new Int8Array(event.target.result));
        this._invalidate();
      };
      file_reader.onerror = () => {console.error('Unable to read file ' + file.name + '.');};
      file_reader.readAsArrayBuffer(files[0]);
    }
  }

  Program.prototype._load_file = function (a_data) {
    const start_time = new Date();
    const num_bytes = a_data.length * a_data.BYTES_PER_ELEMENT;
    const data_ptr = Module._malloc(num_bytes);
    const data_on_heap = new Int8Array(Module.HEAP8.buffer, data_ptr, num_bytes);
    data_on_heap.set(a_data);
    Module.app_load_file(data_on_heap.byteOffset,a_data.length);
    Module._free(data_ptr);
    console.log('[Copy to Heap (Cwrap)] Time to load: ' + (new Date() - start_time));
  }

  /*
  window.addEventListener('resize', resize_canvas, false);
  function resize_canvas() {
            htmlCanvas.width = window.innerWidth;
            htmlCanvas.height = window.innerHeight;
  }*/

  Program.prototype._download_out_png = function() {
    var length = Module.app_get_doc_file_size();
    //console.log('debug : length '+length);      
    if(length==0) {alert("File not found.");return;}
    var buffer = Module.app_get_doc_file();
    const array = new Int8Array(Module.HEAP8.buffer,buffer,length);
    download(array,"out.png","example/binary");
    Module.app_delete_buffer(buffer);      
  }
    
  app_main.run = function (container, canvas) {new Program(container, canvas);};

})(app_main || (app_main = {}));
