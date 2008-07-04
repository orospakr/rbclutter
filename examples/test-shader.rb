#!/usr/bin/ruby

# port of test-shaders.c from the Clutter source

require 'clutter'

Clutter::init

# a couple of boilerplate defines that are common amongst all the
# sample shaders

# FRAGMENT_SHADER_BEGIN: generate boilerplate with a local vec4 color
# already initialized, from a sampler2D in a variable tex.

FRAGMENT_SHADER_BEGIN = <<EOF
uniform sampler2D tex;
uniform float x_step, y_step;

void main ()
{
  vec4 color = texture2D (tex, vec2 (gl_TexCoord[0].st));
EOF

# FRAGMENT_SHADER_END: apply the changed color to the output buffer
# correctly blended with the gl specified color (makes the opacity of
# actors work correctly).

FRAGMENT_SHADER_END = <<EOF
  gl_FragColor = color;
  gl_FragColor = gl_FragColor * gl_Color;
}
EOF

SHADERS =
  [
   [ "brightness-contrast",
     <<EOF
uniform float brightness, contrast;
#{FRAGMENT_SHADER_BEGIN}
color.rgb = (color.rgb - vec3(0.5, 0.5, 0.5)) * contrast
            + vec3 (brightness + 0.5, brightness + 0.5, brightness + 0.5);
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "box-blur",
     <<EOF
vec4 get_rgba_rel(sampler2D tex, float dx, float dy)
{
  return texture2D (tex, gl_TexCoord[0].st + vec2(dx,dy) * 2.0);
}

#{FRAGMENT_SHADER_BEGIN}
  float count = 1.0;
  color += get_rgba_rel (tex, -x_step, -y_step); count++;
  color += get_rgba_rel (tex, -x_step,  0.0);    count++;
  color += get_rgba_rel (tex, -x_step,  y_step); count++;
  color += get_rgba_rel (tex,  0.0,    -y_step); count++;
  color += get_rgba_rel (tex,  0.0,     0.0);    count++;
  color += get_rgba_rel (tex,  0.0,     y_step); count++;
  color += get_rgba_rel (tex,  x_step, -y_step); count++;
  color += get_rgba_rel (tex,  x_step,  0.0);    count++;
  color += get_rgba_rel (tex,  x_step,  y_step); count++;
  color = color / count;
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "invert",
     <<EOF
#{FRAGMENT_SHADER_BEGIN}
  color.rgb = vec3(1.0, 1.0, 1.0) - color.rgb;\n
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "brightness-contrast",
     <<EOF
uniform float brightness;
uniform float contrast;
#{FRAGMENT_SHADER_BEGIN}
  color.r = (color.r - 0.5) * contrast + brightness + 0.5;
  color.g = (color.g - 0.5) * contrast + brightness + 0.5;
  color.b = (color.b - 0.5) * contrast + brightness + 0.5;
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "gray",
     <<EOF
#{FRAGMENT_SHADER_BEGIN}
  float avg = (color.r + color.g + color.b) / 3.0;
  color.r = avg;
  color.g = avg;
  color.b = avg;
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "combined-mirror",
     <<EOF
#{FRAGMENT_SHADER_BEGIN}
  vec4 colorB = texture2D (tex, vec2(gl_TexCoord[0].ts));
  float avg = (color.r + color.g + color.b) / 3.0;
  color.r = avg;
  color.g = avg;
  color.b = avg;
  color = (color + colorB)/2.0;
#{FRAGMENT_SHADER_END}
EOF
   ],

   [ "edge-detect",
     <<EOF
float get_avg_rel(sampler2D texB, float dx, float dy)
{
  vec4 colorB = texture2D (texB, gl_TexCoord[0].st + vec2(dx, dy));
  return (colorB.r + colorB.g + colorB.b) / 3.0;
}

#{FRAGMENT_SHADER_BEGIN}
  mat3 sobel_h = mat3( 1.0,  2.0,  1.0,
                       0.0,  0.0,  0.0,
                      -1.0, -2.0, -1.0);
  mat3 sobel_v = mat3( 1.0,  0.0, -1.0,
                       2.0,  0.0, -2.0,
                       1.0,  0.0, -1.0);
  mat3 map = mat3( get_avg_rel(tex, -x_step, -y_step),
                   get_avg_rel(tex, -x_step, 0.0),
                   get_avg_rel(tex, -x_step, y_step),
                   get_avg_rel(tex, 0.0, -y_step),
                   get_avg_rel(tex, 0.0, 0.0),
                   get_avg_rel(tex, 0.0, y_step),
                   get_avg_rel(tex, x_step, -y_step),
                   get_avg_rel(tex, x_step, 0.0),
                   get_avg_rel(tex, x_step, y_step) );
  mat3 gh = sobel_h * map;
  mat3 gv = map * sobel_v;
  float avgh = (gh[0][0] + gh[0][1] + gh[0][2] +
                gh[1][0] + gh[1][1] + gh[1][2] +
                gh[2][0] + gh[2][1] + gh[2][2]) / 18.0 + 0.5;
  float avgv = (gv[0][0] + gv[0][1] + gv[0][2] +
                gv[1][0] + gv[1][1] + gv[1][2] +
                gv[2][0] + gv[2][1] + gv[2][2]) / 18.0 + 0.5;
  float avg = (avgh + avgv) / 2.0;
  color.r = avg * color.r;
  color.g = avg * color.g;
  color.b = avg * color.b;
#{FRAGMENT_SHADER_END}
EOF
     ]
  ]

$shader_no = 0

def set_shader(actor, new_no)
  if new_no >= 0 && new_no < SHADERS.length
    $shader_no = new_no
      
    print("settings shaders[#{$shader_no}] " \
          "named '#{SHADERS[$shader_no][0]}'\n")

    shader = Clutter::Shader.new
    shader.fragment_source = SHADERS[$shader_no][1]

    # try to bind the shader, provoking an error we catch if there are
    # issues with the shader sources we've provided. At a later stage
    # it should be possible to iterate through a set of alternate
    # shader sources (glsl -> asm -> cg?) and the one that succesfully
    # compiles is used.

    actor.shader = nil

    begin
      shader.compile
    rescue Clutter::ShaderError => error
      print("unable to set shaders[#{$shader_no}] " \
            "named '#{SHADERS[$shader_no][0]}': #{error.message}")
    else
      actor.shader = shader
      actor.set_shader_param("radius", 3.0)
      actor.set_shader_param("brightness", 0.4)
      actor.set_shader_param("contrast", -1.9)

      actor.set_shader_param("x_step", 1.0 / actor.width)
      actor.set_shader_param("y_step", 1.0 / actor.height)
    end
  end 
end

def button_release_cb(actor, event)
  case event.button
  when 1
    new_no = $shader_no - 1
  else
    new_no = $shader_no + 1
  end

  set_shader(actor, new_no)

  false
end

Clutter::init

stage = Clutter::Stage.get_default
stage.set_size(512, 384)

print("applying shaders[#{$shader_no}] named '#{SHADERS[$shader_no][0]}'\n")

stage.title = "Shader Test"
stage.color = Clutter::Color.new(0x61, 0x64, 0x8c)

# Create a timeline to manage animation
timeline = Clutter::Timeline.new(360, 60) # num frames, fps
timeline.loop = true                      # have it loop

actor = Clutter::Texture.new
actor.filename = "redhand.png"

set_shader(actor, $shader_no)

actor.set_position(100, 100)

stage << actor

actor.set_shader_param("brightness", 0.4)
actor.set_shader_param("contrast", -1.9)

actor.reactive = true                                 

actor.signal_connect("button-release-event") do |actor, event| 
  button_release_cb(actor, event)
end

stage.show

timeline.start

Clutter::main
