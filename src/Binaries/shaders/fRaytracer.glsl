#version 150 
// FragmentProgram
const int raytraceDepth = 4;
const float eps  = 0.0001;

uniform mat4 ModelView;

const int maxNumberOfLights = 5;
uniform int uNumberOfLights;
uniform vec3 LightAmbient, uLightPositions[maxNumberOfLights], uLightDiffuse[maxNumberOfLights], uLightSpecular[maxNumberOfLights];

uniform int uNumOfTriangle;
uniform int uNumOfTriangleVectors;

const int maxBoundingBoxes = 5;
uniform int uNumOfBoundingBoxes;
uniform int uBoundingBoxIndexes[maxBoundingBoxes + 1];
uniform vec3 uMinBoundingBox[maxBoundingBoxes];
uniform vec3 uMaxBoundingBox[maxBoundingBoxes];

uniform samplerBuffer bufferData;

uniform int ftime;

float piover4 = atan(1.0);

const float dlight = 0.025;
float lightness = 0.5;

in vec3 org,dir;

struct Ray
{
	vec3 org;
	vec3 dir;
	float rIndex;
};

struct Sphere
{
	vec3 c;
	float r;
	vec3 diffuse;
};

struct Plane
{
	vec3 p;
	vec3 n;
	vec3 diffuse;
};

struct Triangle
{
	vec3 a;
	vec3 b;
	vec3 c;
	vec3 n;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflect;
	float refract;
};

struct Intersection
{
	float t;
	vec3 p;	 // hit point
	vec3 n;	 // normal
	int hit;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflect;
	float refract;
};

void asdftriangle_intersect(in Triangle triangle, in Ray ray, inout Intersection isect, int back)
{
    vec3 edge1 = triangle.b - triangle.a;
    vec3 edge2 = triangle.c - triangle.a;
    
    vec3 pvec = cross(ray.dir, edge2);
    float det = dot(edge1, pvec);
    
    if (det < eps) return;
    float invDet = 1 / det;
    
    vec3 tvec = ray.org - triangle.a;
    
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return;
    
    vec3 qvec = cross(tvec, edge1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0 || u + v > 1) return;
    
    float t = dot(edge2, qvec) * invDet;
    
    if((t > 0.0) && (t < isect.t)) {
		// Point of intersection on the plane
		vec3 p = vec3(ray.org.x + t * ray.dir.x,
					  ray.org.y + t * ray.dir.y,
					  ray.org.z + t * ray.dir.z);

		isect.hit = 1;
		isect.t   = t;
		isect.n   = triangle.n;
		isect.p   = p;
		isect.diffuse = triangle.diffuse;
		isect.ambient = triangle.ambient;
		isect.specular = triangle.specular;
		isect.shininess = triangle.shininess;
		isect.reflect = triangle.reflect;
		isect.refract = triangle.refract;
	}
}

void triangle_intersect(in Triangle triangle, in Ray ray, inout Intersection isect, int back)
{
	vec3 e2 = triangle.c - triangle.a;      // second edge
	vec3 e1 = triangle.b - triangle.a;      // first edge
	vec3 r = cross(ray.dir, e2);  			// (d X e2) is used two times in the formula
                    						// so we store it in an appropriate vector
	vec3 s = ray.org - triangle.a;     		// translated ray origin
	float a = dot(e1, r);    				// a=(d X e2)*e1
	float f = 1 / a;           				// slow division*
	vec3 q = cross(s,e1);
    float u = dot(s,r);
    
	//bool frontfacing=true;
	if (a > eps && back == 0)            // a very small number
	{ 
		// Front facing triangle...
	    if (u < 0 || u > a) return;
	    float v = dot(ray.dir, q);
	    if (v < 0 || u + v > a) return;
	    
    	float t = f*dot(e2,q);

	    if((t > 0.0) && (t < isect.t)) {
			// Point of intersection on the plane
			vec3 p = vec3(ray.org.x + t * ray.dir.x,
						  ray.org.y + t * ray.dir.y,
						  ray.org.z + t * ray.dir.z);
	
			isect.hit = 1;
			isect.t   = t;
			isect.n   = triangle.n;
			isect.p   = p;
			isect.diffuse = triangle.diffuse;
			isect.ambient = triangle.ambient;
			isect.specular = triangle.specular;
			isect.shininess = triangle.shininess;
			isect.reflect = triangle.reflect;
			isect.refract = triangle.refract;
		}
	}
  	else if (a < -eps && back == 1)
  	{ 
  		// Back facing triangle...
    	//frontfacing=false;
    	if (u > 0 || u < a) return;
	    float v = dot(ray.dir, q);
	    if (v > 0 || u + v < a) return;
	    
    	float t = f*dot(e2,q);

	    if((t > 0.0) && (t < isect.t)) {
			// Point of intersection on the plane
			vec3 p = vec3(ray.org.x + t * ray.dir.x,
						  ray.org.y + t * ray.dir.y,
						  ray.org.z + t * ray.dir.z);
	
			isect.hit = 1;
			isect.t   = t;
			isect.n   = triangle.n;
			isect.p   = p;
			isect.diffuse = triangle.diffuse;
			isect.ambient = triangle.ambient;
			isect.specular = triangle.specular;
			isect.shininess = triangle.shininess;
			isect.reflect = triangle.reflect;
			isect.refract = triangle.refract;
		}
	    
  	} else return; // Ray parallel to triangle plane
  	

}

void plane_intersect(in Plane pl, in Ray ray, inout Intersection isect)
{
	// d = -(p . n)
	// t = -(ray.org . n + d) / (ray.dir . n)
	float d = -dot(pl.p, pl.n);
	float v = dot(ray.dir, pl.n);

	if (abs(v) < 1.0e-6)
		return; // the plane is parallel to the ray.

	float t = -(dot(ray.org, pl.n) + d) / v;

	if ( (t > 0.0) && (t < isect.t) )
	{
		isect.hit = 1;
		isect.t   = t;
		isect.n   = pl.n;

		vec3 p = vec3(ray.org.x + t * ray.dir.x,
					  ray.org.y + t * ray.dir.y,
					  ray.org.z + t * ray.dir.z);
		isect.p = p;
		float offset = 0.2;
		vec3 dp = p + offset;
		if ((mod(dp.x, 1.0) > 0.5 && mod(dp.z, 1.0) > 0.5)
		||  (mod(dp.x, 1.0) < 0.5 && mod(dp.z, 1.0) < 0.5))
			isect.diffuse = pl.diffuse;
		else
			isect.diffuse = pl.diffuse * 0.5;
	}
}

void buildTriangle(inout Triangle t, vec3 a, vec3 b, vec3 c, vec3 diffuse, vec3 ambient, vec3 specular, vec3 material, vec3 normal) {
	t.a = a;
	t.b = b;
	t.c = c;
	t.diffuse = diffuse;
	t.ambient = ambient;
	t.specular = specular;
	t.shininess = material.x;
	t.reflect = material.y;
	t.refract = material.z;
	t.n = normal;
}

int HitBox(in Ray r, vec3 minBound, vec3 maxBound) {

	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (r.dir.x >= 0) {
		tmin = (minBound.x - r.org.x) / r.dir.x;
		tmax = (maxBound.x - r.org.x) / r.dir.x;
	}
	else {
		tmin = (maxBound.x - r.org.x) / r.dir.x;
		tmax = (minBound.x - r.org.x) / r.dir.x;
	}
	if (r.dir.y >= 0) {
		tymin = (minBound.y - r.org.y) / r.dir.y;
		tymax = (maxBound.y - r.org.y) / r.dir.y;
	}
	else {
		tymin = (maxBound.y - r.org.y) / r.dir.y;
		tymax = (minBound.y - r.org.y) / r.dir.y;
	}
	if ( (tmin > tymax) || (tymin > tmax) || (tmax < eps) || (tymax < eps) )
		return 0;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (r.dir.z >= 0) {
		tzmin = (minBound.z - r.org.z) / r.dir.z;
		tzmax = (maxBound.z - r.org.z) / r.dir.z;
	}
	else {
		tzmin = (maxBound.z - r.org.z) / r.dir.z;
		tzmax = (minBound.z - r.org.z) / r.dir.z;
	}
	
	if ( (tmin > tzmax) || (tzmin > tmax) || (tzmax < eps))
		return 0;
	
	return 1;
}

void Intersect(in Ray r, inout Intersection i, int back)
{
	/*for (int c = 0; c < uNumOfSpheres; c++)
	{
		sphere_intersect(c, r, i, back);
	}*/
	
	Triangle triangle;
	int index = uNumOfTriangle * uNumOfTriangleVectors, endTriangleIndex = index;


	//normal modulation guts
	vec3 v1=vec3(1,0,0), v2, v3, v4;
	mat3 m1,m2,tmp;
	float mcos,msin;
	mat4 fromUnit;
	
	float rotx=piover4*cos(ftime/125.0)/8.0;
	float rotz=piover4*sin(ftime/125.0)/8.0;
	float crotx=cos(rotx),srotx=sin(rotx),crotz=cos(rotz),srotz=sin(rotz);
	float rotmag,cosmagx,sinmagx,cosmagz,sinmagz;

	vec3 centerPoint, rs, triangleNormal, temp;
	float radius, radiusSquared, B, C, D, sqrtOfD, t, tPlus;

	for(int hitBoxIndex = 0; hitBoxIndex < uNumOfBoundingBoxes; hitBoxIndex++)
	{
		if(HitBox(r, texelFetch(bufferData, index).xyz, texelFetch(bufferData, index+1).xyz) == 1) 
		{
			vec4 temp = texelFetch(bufferData, index+2);
			int pointIndex = int(temp.x) * uNumOfTriangleVectors;

			for (int c = int(temp.x); c < int(temp.y); c++) 
			{
				centerPoint = (texelFetch(bufferData, pointIndex+8)).xyz;
				radius = texelFetch(bufferData, pointIndex+9).x;
				radiusSquared = texelFetch(bufferData, pointIndex+9).y;
				rs = r.org - centerPoint;
				B = dot(rs, r.dir);
				C = dot(rs, rs) - radiusSquared;
				D = B * B - C;
			
				if (D > 0.0) {
					sqrtOfD = sqrt(D);
					tPlus = -B + sqrtOfD;
					
					if(tPlus > 0.0)
					{
						triangleNormal = texelFetch(bufferData, pointIndex+7).xyz;
						
						//////////////////////////////////////
						//modulation of triangle normals
						//////////////////////////////////////
						  
						//find transformation of (1,0,0) that results in the triangleNormal
						/*v2=triangleNormal;
						v3=normalize(cross(v1,v2));
						v4=cross(v4,v1);
						m1=mat3(v1,v4,v3);
						mcos=dot(v2,v1);
						msin=dot(v2,v4);
						m2=mat3(mcos, msin, 0,
						        -msin, mcos, 0,
						        0, 0, 1);
						tmp=inverse(m1)*m2*m1;
						fromUnit=mat4(vec4(tmp[0], 0.0), vec4(tmp[1], 0.0), vec4(tmp[2], 0.0), vec4(0.0, 0.0, 0.0, 1.0));              
						  
						mat4(vec4(1.0,0.0,0.0,0.0),vec4(0.0,cos(rotx),sin(rotx),0.0),vec4(0.0,-sin(rotx),cos(rotx),0.0),vec4(0.0,0.0,0.0,1.0)) *
						                  mat4(vec4(cos(rotz),sin(rotz),0.0,0.0),vec4(-sin(rotz),cos(rotz),0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(0.0,0.0,0.0,1.0)) * 
						                  fromUnit * vec4(1.0,0.0,0.0,1.0)
						*/
						  
						/*cosmagx=cos(atan(triangleNormal[1]/triangleNormal[0]));
						sinmagx=sin(atan(triangleNormal[1]/triangleNormal[0]));
						cosmagz=cos(atan(triangleNormal[1]/triangleNormal[2]));
						sinmagz=sin(atan(triangleNormal[1]/triangleNormal[2]));*/
						vec3 ownage = (ModelView * vec4(org,1.0)).xyz;;
						cosmagx=sinmagx=cosmagz=sinmagz=sqrt((ownage[0]-centerPoint[0])*(ownage[0]-centerPoint[0])+(ownage[1]-centerPoint[1])*(ownage[1]-centerPoint[1]));
						//cosmagx=sinmagx=cosmagz=sinmagz=1.0;
						  
						//do some noodling
						triangleNormal = normalize(mat3(vec3(1.0,0.0,0.0),vec3(0.0,cosmagx*crotx,sinmagx*srotx),vec3(0.0,-sinmagx*srotx,cosmagx*crotx)) *
						                 mat3(vec3(cosmagx*crotx,0.0,sinmagz*srotz),vec3(0.0,1.0,0.0),vec3(-sinmagx*srotx,0.0,cosmagz*crotz)) *
						                 mat3(vec3(cosmagz*crotz,sinmagz*srotz,0.0),vec3(-sinmagz*srotz,cosmagz*crotz,0.0),vec3(0.0,0.0,1.0)) 
						                  * triangleNormal);
						                    
						  
						///////////////////////////////////////
						//END of modulation of triangle normals
						///////////////////////////////////////

						buildTriangle(triangle, texelFetch(bufferData, pointIndex).xyz, texelFetch(bufferData, pointIndex+1).xyz, texelFetch(bufferData, pointIndex+2).xyz, 
															texelFetch(bufferData, pointIndex+3).xyz, texelFetch(bufferData, pointIndex+4).xyz, texelFetch(bufferData, pointIndex+5).xyz,
															texelFetch(bufferData, pointIndex+6).xyz,
															triangleNormal
															);
						triangle_intersect(triangle, r, i, back);
						
					}
				}
				
				pointIndex += uNumOfTriangleVectors;
			}
		}
		index += 3;
	}
}

vec3 computeLightShadow(in Intersection isect)
{
	int i, j;
	int ntheta = 16;
	int nphi   = 16;
	float eps  = 0.0001;

	// Slightly move ray org towards ray dir to avoid numerical probrem.
	vec3 p = vec3(isect.p.x + eps * isect.n.x,
				  isect.p.y + eps * isect.n.y,
				  isect.p.z + eps * isect.n.z);

	vec3 lightPoint = uLightPositions[0];
	Ray ray;
	ray.org = p;
	ray.dir = normalize(lightPoint - p);
	
	Intersection lisect;
	lisect.hit = 0;
	lisect.t = 1.0e+30;
	lisect.n = lisect.p = lisect.diffuse = vec3(0, 0, 0);
	Intersect(ray, lisect, 0);
	if (lisect.hit != 0)
		return vec3(0.0, 0.0, 0.0);
	else
	{
		//Phong shading
		vec3 L = normalize(lightPoint - p);
	    vec3 E = normalize(-p);
	    vec3 H = normalize(L + E);
	    vec3 R = normalize(-reflect(L, isect.n));
	    
	    float Kd = max( dot(L, isect.n), 0.0 );
	    vec3 diffuse = Kd * uLightDiffuse[0] * isect.diffuse;
		
	    float Ks = pow( max(dot(isect.n, R), 0.0), isect.shininess);
	    vec3  specular = Ks * uLightSpecular[0] * isect.specular;
	    if( dot(L, isect.n) < 0.0) {
	        specular = vec3(0.0, 0.0, 0.0);
	    }
	    
		return diffuse + specular;
	}
}

void asdfmain()
{
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	
	vec4 color = vec4(0, 0, 0, 1);
	
	int index = uNumOfTriangle * uNumOfTriangleVectors, endTriangleIndex = index;
	
	for(int hitBoxIndex = 0; hitBoxIndex < uNumOfBoundingBoxes; hitBoxIndex++)
	{
		if(HitBox(r, texelFetch(bufferData, index).xyz, texelFetch(bufferData, index+1).xyz) == 1) 
		{
			color.rgb = vec3(1, 1, 1);
		}
		
		index += 3;
	}
	
	gl_FragColor = color;
}

// test intersection functions
void test_main()
{
	Sphere sphere;
	sphere.c   = (vec4(0.0, 0.0, -2.0, 1.0)).xyz;
	sphere.r   = 0.5;
	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 diffuse = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.diffuse = vec3(0, 0, 0);
		
	//sphere_intersect(sphere, r, i);
	if (i.hit != 0)
		diffuse = vec4(1,0,0,1);
		
	gl_FragColor = diffuse;
}

void main()
{	
  gl_FragColor = vec4(1.0,1.0,1.0,1.0);
  return;

	Ray r, r2;
	r.org = org;
	r.dir = normalize(dir);
	r2.org = org;
	r2.dir = normalize(dir);
	
	vec4 color = vec4(0, 0, 0, 1);
	
	Intersection isect;
	vec3 bcolor = vec3(1,1,1);

	vec3 reflectColor = vec3(0, 0, 0);
	vec3 refractColor = vec3(0, 0, 0);

	for (int k = 0; k < raytraceDepth; k++)
	{
		isect.hit = 0;
		isect.t = 1.0e+30;
		isect.n = isect.p = isect.diffuse = vec3(0, 0, 0);
			
		Intersect(r, isect, 0);
		if (isect.hit != 0)
		{
			reflectColor += isect.ambient + bcolor * computeLightShadow(isect);
			bcolor *= isect.reflect * isect.diffuse;
		}
		else
		{
			break;
		}
		
		if(isect.reflect < eps) break;

		r.org = vec3(isect.p.x + eps * isect.n.x,
					 isect.p.y + eps * isect.n.y,
					 isect.p.z + eps * isect.n.z);
		r.dir = reflect(r.dir, vec3(isect.n.x, isect.n.y, isect.n.z));
	}

	int back = 0;
	float rIndex = 0.0;
	for (int k = 0; k < raytraceDepth; k++)
	{
		isect.hit = 0;
		isect.t = 1.0e+30;
		isect.n = isect.p = isect.diffuse = vec3(0, 0, 0);
		isect.refract = 0.0;
		Intersect(r2, isect, back);
		
		if(isect.refract > eps) 
		{
			float n = rIndex / isect.refract;
			vec3 N = isect.n;
			rIndex = isect.refract;
			
			if(back == 1) N = isect.n * -1;

			float cosI = -dot( N, r2.dir );
			float cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
			if (cosT2 > 0.0)
			{
				r2.dir = (n * r2.dir) + (n * cosI - sqrt( cosT2 )) * N;
				r2.org = vec3(isect.p.x + eps * isect.n.x,
							 isect.p.y + eps * isect.n.y,
							 isect.p.z + eps * isect.n.z);
			}


			r2.org = vec3(isect.p.x + eps * isect.n.x,
						 isect.p.y + eps * isect.n.y,
						 isect.p.z + eps * isect.n.z);

			if(back == 0)
			{
				back = 1;
			}
			else 
			{
				back = 0;
				if(rIndex > eps)
				{
					isect.hit = 0;
					isect.t = 1.0e+30;
					isect.n = isect.p = isect.diffuse = vec3(0, 0, 0);
					isect.refract = 0.0;
					Intersect(r2, isect, 0);
					
					refractColor += computeLightShadow(isect);
				}
				
				rIndex = 0.0;
			}
		}
		else
		{
			break;
		}
	}
	
	color.rgb = reflectColor + refractColor;

	gl_FragColor = color;
}
