#!/bin/python3

output_vert = ''
output_frag = ''

VERTEX   = 0
FRAGMENT = 1

BASE_FOLDER = "./shaders/"

def shader_load(path, s_type):
    print("Loading shader "+path)
    output = ''
    with open(path, "r") as shader:
        data = shader.read()
        tokens = data.split()
        for i in range(len(tokens)):
            if (tokens[i] == "#include"):
                newpath = tokens[i+1]
                data = data.replace("{} {}".format(tokens[i], newpath), shader_load(BASE_FOLDER+newpath, s_type))
                print("Including shader "+BASE_FOLDER+newpath)
        output = data
    shader.close()
    return output

# Vertex shader
output_vert = shader_load(BASE_FOLDER+"m_vert.glsl", VERTEX)
with open(BASE_FOLDER+"pp/final_vert.glsl", "w") as final_vert:
    final_vert.write(output_vert)
final_vert.close()

# Fragment shader
output_frag = shader_load(BASE_FOLDER+"m_frag.glsl", FRAGMENT)
with open(BASE_FOLDER+"pp/final_frag.glsl", "w") as final_frag:
    final_frag.write(output_frag)
final_frag.close()

print("Done")
