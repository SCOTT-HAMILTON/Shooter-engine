uniform sampler2D texture;
uniform float y_axe = 500;
uniform vec4 first_color = vec4(255);
uniform vec4 second_color = vec4(0);

int mod(int a, int b){
	return int(a - int(b * int(trunc(a/b))));
}

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    int min = int(y_axe)-10000, max = int(y_axe)+10000;
    int pos_y = int(gl_FragCoord.y);
    int y_axe_int = int(y_axe);
    int  coeff_first = int(trunc((pos_y-mod(pos_y,y_axe_int)+y_axe_int)/y_axe_int-1));
    int coeff_second = int(trunc(1-coeff_first));
    gl_FragColor[0] = (first_color[0]*coeff_first)+(second_color[0]*coeff_second);
    gl_FragColor[1] = (first_color[1]*coeff_first)+(second_color[1]*coeff_second);
    gl_FragColor[2] = (first_color[2]*coeff_first)+(second_color[2]*coeff_second);
	int tmp = int(int(dot(pixel.xyz, pixel.xyz))%17581375);
	gl_FragColor[3] = (tmp/tmp)*255;
    if (dot(pixel.xyz, pixel.xyz) == 0.0)
    	gl_FragColor = vec4(0, 0, 0, 0);
}

