// Original author: Spatial (https://stackoverflow.com/users/2434130/spatial)
// License: CC BY-SA 3.0 (https://creativecommons.org/licenses/by-sa/3.0/)
// Link: https://stackoverflow.com/a/17479300

#version 460

uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

float rand(float x) {
    return floatConstruct(hash(floatBitsToUint(x)));
}

vec2 rand(vec2 v) {
    uvec2 s = floatBitsToUint(v);
    uint x = hash(s);
    uint y = hash(x);
    return vec2(
        floatConstruct(x),
        floatConstruct(y)
    );
}

vec3 rand(vec3 v) {
    uvec3 s = floatBitsToUint(v);
    uint x = hash(s);
    uint y = hash(x);
    uint z = hash(y);
    return vec3(
        floatConstruct(x),
        floatConstruct(y),
        floatConstruct(z)
    );
}

vec4 rand(vec4 v) {
    uvec4 s = floatBitsToUint(v);
    uint x = hash(s);
    uint y = hash(x);
    uint z = hash(y);
    uint w = hash(z);
    return vec4(
        floatConstruct(x),
        floatConstruct(y),
        floatConstruct(z),
        floatConstruct(w)
    );
}

