const float pi = 3.1415926535897932384626433832795;

const float earthRadius = 1073741824.0 / 2.0;
const float atmosphereRadiusRelative = 8000000.0;
const float atmosphereRadius = earthRadius + atmosphereRadiusRelative;

const vec3 planetPosition = vec3( 0.0 );

int numSamples = 16;
int numSamplesLight = 8;

//const float rayleightHeight = 20000000.0;
const float rayleightHeight = 800000.0;
//const vec3 rayleighExtinctionAtSeaLevel = vec3( 5.5e-8, 13.0e-8, 22.4e-8 );
//const vec3 rayleighExtinctionAtSeaLevel = vec3( 5.8e-8, 13.5e-8, 33.1e-8 );
uniform vec3 rayleighExtinctionAtSeaLevel;
uniform vec3 mieExtinctionAtSeaLevel;

//const float mieHeight = 120000.0;
const float mieHeight = 120000.0;
//const vec3 mieExtinctionAtSeaLevel = vec3( 21e-8 );
//const vec3 mieExtinctionAtSeaLevel = vec3( 40e-8 );

const float SunIntensity = 20.0;

float rayleighPhase( float mu )
{
	return 3.0 / ( 16.0 * pi ) * ( 1.0 + mu * mu );
}

float miePhase( float mu )
{
	const float g = 0.76;
	const float g2 = g * g;

	return 3.0 / ( 8.0 * pi ) * ( ( 1 - g2 ) * ( 1 + mu * mu ) ) / ( ( 2 + g2 ) * pow( 1 + g2 - 2 * g * mu, 1.5 ) );

	/*const float g = 0.76;
	const float g2 = g * g;

	float a = 3.0 / ( 8.0 * pi );
	float bnum = ( 1 - g2 ) * ( 1 + mu * mu );
	float bden = ( 2 + g2 ) * pow( 1 + g2 - 2.0 * g * mu, 1.5 );  

	return a * bnum / bden;*/
}

vec3 getPointOnAtmosphere( vec3 pos, vec3 dir )
{
	vec3 centerToPosition = planetPosition - pos;

	float d = dot( centerToPosition, dir );

	vec3 h = pos + dir * d;

	float c = atmosphereRadius;
	float b = length( h - planetPosition );
	float a = sqrt( c * c - b * b );

	return pos + dir * ( d + a );
}

bool intersectsWithAtmosphere( vec3 pos, vec3 dir )
{
	vec3 centerToPosition = planetPosition - pos;

	float d = dot( centerToPosition, dir );
	
	vec3 h = pos + dir * d;

	float c = atmosphereRadius;
	float b = length( h - planetPosition );
	float a = sqrt( c * c - b * b );

	if( b > atmosphereRadius )
		return false;

	float tstart = d - a;
	float tend = d + a;

	if( tstart < 0.0 && tend < 0.0 )
		return false;
	
	return true;
}

vec3 getNearestVertex( vec3 pos, vec3 dir )
{
	vec3 centerToPosition = planetPosition - pos;

	float d = dot( centerToPosition, dir );

	vec3 h = pos + dir * d;

	float c = atmosphereRadius;
	float b = length( h - planetPosition );
	float a = sqrt( c * c - b * b );

	return pos + dir * ( d - a );
}

vec3 getFarthestVertex( vec3 pos, vec3 dir )
{
	vec3 centerToPosition = planetPosition - pos;

	float d = dot( centerToPosition, dir );

	vec3 h = pos + dir * d;

	float c = atmosphereRadius;
	float b = length( h - planetPosition );
	float a = sqrt( c * c - b * b );

	return pos + dir * ( d + a );
}

vec3 getSkyColor3( vec3 lightDir, vec3 viewDir, out float opticalDepthR, out float opticalDepthM )
{
	float factorR = 1.0;
	float factorM = 1.0;

	opticalDepthR = 0.0;
	opticalDepthM = 0.0;

	if( !intersectsWithAtmosphere( cameraPos, viewDir ) )
		return vec3( 0.0 );

	float mu = dot( lightDir, viewDir );

	float phaseR = rayleighPhase( mu );
	float phaseM = miePhase( mu );

	vec3 betaR = rayleighExtinctionAtSeaLevel;
	vec3 betaM = mieExtinctionAtSeaLevel;

	vec3 sumR = vec3( 0.0 );
	vec3 sumM = vec3( 0.0 );

	vec3 start = cameraPos;

	if( length( cameraPos ) > atmosphereRadius )
	{
		start = getNearestVertex( cameraPos, viewDir );
	}
	
	vec3 end = Position;

	if( length( Position ) > atmosphereRadius )
	{
		end = getFarthestVertex( Position, viewDir );
	}

	float sampleLength = length( end - start ) / numSamples;

	float current = 0.5 * sampleLength;

	for( int i = 0; i < numSamples; i++ )
	{
		vec3 samplePos = start + viewDir * current;

		float height = length( samplePos ) - earthRadius;

		float hr = exp( -height / rayleightHeight ) * sampleLength;
		float hm = exp( -height / mieHeight ) * sampleLength;

		opticalDepthR += hr;
		opticalDepthM += hm;

		vec3 lightVertex = getPointOnAtmosphere( samplePos, lightDir );

		float sampleLightLength = length( lightVertex - samplePos ) / numSamplesLight;

		float opticalDepthLightR = 0.0;
		float opticalDepthLightM = 0.0;

		float currentLight = 0.5 * sampleLightLength;

		int j = 0;

		for( ; j < numSamplesLight; j++ )
		{
			vec3 samplePosLight = samplePos + lightDir * currentLight;

			float lightHeight = length( samplePosLight ) - earthRadius;
			if( lightHeight < 0 )
				break;

			opticalDepthLightR += exp( -lightHeight / rayleightHeight ) * sampleLightLength;
			opticalDepthLightM += exp( -lightHeight / mieHeight ) * sampleLightLength;

			currentLight += sampleLightLength;
		}

		if( j == numSamplesLight )
		{
			vec3 tau = betaR * ( opticalDepthR + opticalDepthLightR ) * factorR + betaM * 1.1 * ( opticalDepthM + opticalDepthLightM ) * factorM;
			vec3 attenuation = exp( -tau );

			sumR += hr * attenuation;
			sumM += hm * attenuation;
		}

		current += sampleLength;
	}

	vec3 skyColor = sumR * phaseR * betaR * factorR + sumM * phaseM * betaM * factorM * 1.1;
	return SunIntensity * skyColor;
}

vec3 getSkyColor2( vec3 lightDir, out float opticalDepthR, out float opticalDepthM )
{
	vec3 viewDir = normalize( Position - cameraPos );
	return getSkyColor3( lightDir, viewDir, opticalDepthR, opticalDepthM );
}

vec3 getSkyColor( vec3 lightDir )
{
	float a, b;
	return getSkyColor2( lightDir, a, b );
}

vec3 getSkyColor( vec3 start, vec3 end, vec3 lightDir, vec3 viewDir, out float opticalDepthR, out float opticalDepthM )
{
	float factorR = 1.0;
	float factorM = 1.0;

	opticalDepthR = 0.0;
	opticalDepthM = 0.0;

	if( !intersectsWithAtmosphere( cameraPos, viewDir ) )
		return vec3( 0.0 );
	
	float mu = dot( lightDir, viewDir );

	float phaseR = rayleighPhase( mu );
	float phaseM = miePhase( mu );

	vec3 betaR = rayleighExtinctionAtSeaLevel;
	vec3 betaM = mieExtinctionAtSeaLevel;

	vec3 sumR = vec3( 0.0 );
	vec3 sumM = vec3( 0.0 );

	/*vec3 start = cameraPos;

	if( length( cameraPos ) > atmosphereRadius )
	{
		start = getNearestVertex( cameraPos, viewDir );
	}*/
	
	/*vec3 end = Position;

	if( length( Position ) > atmosphereRadius )
	{
		end = getFarthestVertex( Position, viewDir );
	}*/

	float sampleLength = length( end - start ) / numSamples;

	float current = 0.5 * sampleLength;

	for( int i = 0; i < numSamples; i++ )
	{
		vec3 samplePos = start + viewDir * current;

		float height = length( samplePos ) - earthRadius;

		float hr = exp( -height / rayleightHeight ) * sampleLength;
		float hm = exp( -height / mieHeight ) * sampleLength;

		opticalDepthR += hr;
		opticalDepthM += hm;

		vec3 lightVertex = getPointOnAtmosphere( samplePos, lightDir );

		float sampleLightLength = length( lightVertex - samplePos ) / numSamplesLight;

		float opticalDepthLightR = 0.0;
		float opticalDepthLightM = 0.0;

		float currentLight = 0.5 * sampleLightLength;

		int j = 0;

		for( ; j < numSamplesLight; j++ )
		{
			vec3 samplePosLight = samplePos + lightDir * currentLight;

			float lightHeight = length( samplePosLight ) - earthRadius;
			if( lightHeight < 0 )
				break;

			opticalDepthLightR += exp( -lightHeight / rayleightHeight ) * sampleLightLength;
			opticalDepthLightM += exp( -lightHeight / mieHeight ) * sampleLightLength;

			currentLight += sampleLightLength;
		}

		if( j == numSamplesLight )
		{
			vec3 tau = betaR * ( opticalDepthR + opticalDepthLightR ) * factorR + betaM * 1.1 * ( opticalDepthM + opticalDepthLightM ) * factorM;
			vec3 attenuation = exp( -tau );

			sumR += hr * attenuation;
			sumM += hm * attenuation;
		}

		current += sampleLength;
	}

	vec3 skyColor = sumR * phaseR * betaR * factorR + sumM * phaseM * betaM * factorM * 1.1;
	return SunIntensity * skyColor;
}