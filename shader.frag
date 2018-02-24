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
	gl_FragColor = second_color;
    if (gl_FragCoord.y>y_axe){
		gl_FragColor = first_color;
	}
	int tmp = int(int(dot(pixel.xyz, pixel.xyz))%17581375);
	gl_FragColor[3] = (tmp/tmp)*255;
}

