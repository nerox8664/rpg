#version 120

precision highp float;

varying vec2 texture_coordinate;

uniform sampler2D heightTexture;

uniform sampler2D TerrainTexture1;
uniform sampler2D TerrainTexture2;
uniform sampler2D TerrainTexture3;
uniform sampler2D TerrainTexture4;
uniform sampler2D TerrainTexture5;

uniform float time;

mediump float getHeight() {
	return (
	((texture2D(heightTexture, texture_coordinate-vec2(-0.01,-0.01)).r-0.5)*255.0) +
	((texture2D(heightTexture, texture_coordinate-vec2(-0.01,0.00)).r-0.5)*255.0) +
	((texture2D(heightTexture, texture_coordinate-vec2(-0.01,0.01)).r-0.5)*255.0) +

	((texture2D(heightTexture, texture_coordinate-vec2(0.00,-0.01)).r-0.5)*255.0) +
	((texture2D(heightTexture, texture_coordinate-vec2(0.00,0.01)).r-0.5)*255.0) +
	((texture2D(heightTexture, texture_coordinate-vec2(0.01,-0.01) ).r-0.5)*255.0)+

	((texture2D(heightTexture, texture_coordinate-vec2(0.01,0.00) ).r-0.5)*255.0)+
	((texture2D(heightTexture, texture_coordinate-vec2(0.01,0.01)).r-0.5)*255.0) +
	((texture2D(heightTexture, texture_coordinate).r-0.5)*255.0)
	)/9.0;
}

void main (void)
{

	mediump float height = getHeight();

	vec2 texCoords = texture_coordinate + vec2(time, time) ;

	mediump float snow=0, rock=0, grass=0, dirt=0, water=0;

	mediump float x = height;


	dirt = -0.0008*(x-26)*(x-26)+1.0;

	grass = -0.0011*(x-48)*(x-48)+1.0;


	rock = -0.0009*(x-70)*(x-70)+1.0;

	if(x-90 > 0) {
		snow = 0.125*(x-90);
		rock = 1;
	}
	else
		snow = 0;

	if(x <=0 )
		water = 0.12*(-x);


	if(dirt<0) dirt = 0;
	if(grass<0) grass = 0;
	if(rock<0) rock = 0;
	if(snow<0) snow = 0;
	if(water<0) water = 0;
	//if(x>130) snow=1.0;

	if(snow>1) {
		dirt = dirt/snow;
		grass = grass/snow;
		rock = rock/snow;
		snow = snow/snow;
		water = water/snow;
	}
	if(rock>1) {
		dirt = dirt/rock;
		grass = grass/rock;
		rock = rock/rock;
		snow = snow/rock;
		water = water/rock;
	}
	if(grass>1) {
		dirt = dirt/grass;
		grass = grass/grass;
		rock = rock/grass;
		snow = snow/grass;
		water = water/grass;
	}
	if(dirt>1) {
		dirt = dirt/dirt;
		grass = grass/dirt;
		rock = rock/dirt;
		snow = snow/dirt;
		water = water/dirt;
	}
	if(water>1) {
		dirt = dirt/water;
		grass = grass/water;
		rock = rock/water;
		snow = snow/water;
		water = water/water;
	}



	//texCoords=normalize(texCoords);

	gl_FragColor = dirt * texture2D(TerrainTexture1, texture_coordinate);
	gl_FragColor += grass* texture2D(TerrainTexture2, texture_coordinate);
	gl_FragColor += rock * texture2D(TerrainTexture3, texture_coordinate);
	gl_FragColor += snow * texture2D(TerrainTexture4, texture_coordinate);
	gl_FragColor += water * texture2D(TerrainTexture5, texCoords);

	//gl_FragColor = normalize(gl_FragColor);
	gl_FragColor.a = 1.0;

	if(x <=0 ) {
		gl_FragColor *= max( (1-0.012*(-x)) , 0.7);
		gl_FragColor.b = min(1, gl_FragColor.b+0.012*(-x ));
		//
	}
	/*
	if( height < 0 ) {
		gl_FragColor *= pow(3,height/2.0);

		gl_FragColor += min(-height/2.0,1.0)*texture2D(TerrainTexture5, texCoords );

	}

	*/
}
