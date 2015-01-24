
#pragma once
#include <cmath>
#include <iostream>

#define NUM_INF 3e38
#define PI 3.14159265358979323846264338327950288f

inline float floor_(float x) {
	return floor(x);
}

// unused template to typedef vectors, points and normals differently
template<int i>
struct vec_ {
public:
	float x, y, z;

	vec_() : x(.0f), y(.0f), z(.0f) { }
	vec_(const float & val) : x(val), y(val), z(val) { }
	vec_(const float & x, const float & y, const float & z) : x(x), y(y), z(z) {}
	vec_(const vec_ & other) : x(other.x), y(other.y), z(other.z) { }

	template<int j> // only explicit conversion between types allowed
	explicit vec_(const vec_<j> & other) : x(other.x), y(other.y), z(other.z) { }

	bool operator==(const vec_ & other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	vec_ operator+(const vec_ & other) const {
		return vec_(x + other.x, y + other.y, z + other.z);
	}
	vec_ & operator+=(const vec_ & other) {
		x += other.x; y += other.y; z += other.z;
		return *this;
	}
	vec_ operator-() const {
		return vec_(-x, -y, -z);
	}
	vec_ operator-(const vec_ & other) const {
		return vec_(x - other.x, y - other.y, z - other.z);
	}
	vec_ & operator-=(const vec_ & other) {
		x -= other.x; y -= other.y; z -= other.z;
		return *this;
	}
	vec_ operator*(const vec_ & other) const {
		return vec_(x*other.x, y*other.y, z*other.z);
	}
	vec_ operator*(const float v) const {
		return vec_(x*v, y*v, z*v);
	}
	vec_ & operator*=(const float v) {
		x *= v; y *= v; z *= v;
		return *this;
	}
	vec_ operator/(const float v) const {
		float iv = 1.0f / v;
		return vec_(x*iv, y*iv, z*iv);
	}
	vec_ & operator/=(const float v) {
		float iv = 1.0f / v; x *= iv; y *= iv; z *= iv;
		return *this;
	}
	float operator[] (int i) const {
		return (&x)[i];
	}
	float & operator[] (int i) {
		return (&x)[i];
	}
	inline float sqlen() const {
		return x*x + y*y + z*z;
	}
	inline float len() const {
		return sqrt(sqlen());
	}
	inline vec_ & normalize() {
		float iv = 1.0f / len(); x *= iv; y *= iv; z *= iv;
		return *this;
	}
	inline vec_ normalized() const {
		float iv = 1.0f / len();
		return vec_(x*iv, y*iv, z*iv);
	}
	inline vec_ floor() const {
		return vec_(floor_(x), floor_(y), floor_(z));
	}
};

template<int type>
inline vec_<type> operator*(const float& a, const vec_<type>& v) {
	return v*a;
}

template<int type>
inline float dot(const vec_<type>& a, const vec_<type>& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

template<int type>
inline float absdot(const vec_<type>& a, const vec_<type>& b) {
	return abs(dot(a, b));
}

template<int type>
inline float clampdot(const vec_<type>& a, const vec_<type>& b) {
	return max(.0f, dot(a, b));
}

template<int type>
inline vec_<type> cross(const vec_<type>& a, const vec_<type>& b) {
	return vec_<type>(a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x);
}

template<int type>
inline vec_<type> normalize(const vec_<type>& v) {
	return v.normalized();
}

template<int type>
inline void form_base(const vec_<type>& a, vec_<type>& b, vec_<type>& c) {
	if (abs(a.x)>abs(a.y)) {
		float ilen = 1.0f / sqrt(a.x*a.x + a.z*a.z);
		b = vec_<type>(-a.z*ilen, .0f, a.x*ilen);
	}
	else {
		float ilen = 1.0f / sqrt(a.y*a.y + a.z*a.z);
		b = vec_<type>(.0f, a.z*ilen, -a.y*ilen);
	}
	c = cross(a, b);
}

template<int type>
inline std::ostream& operator<<(std::ostream& out, const vec_<type>& a) {
	out << "{ " << a.x << ", " << a.y << ", " << a.z << " }";
	return out;
}

typedef vec_<0> vec;
typedef vec_<1> pos;
typedef vec_<2> nor;

inline vec cosine_hemisphere(const float rnd_u, const float rnd_v) {
	const float r = sqrt(rnd_u),
		x = r*cos(2.0f*PI*rnd_v), y = r*sin(2.0f*PI*rnd_v);
	return vec(x, y, sqrt(1.0f - x*x - y*y));
}

inline vec uniform_sphere(float& pdf, const float rnd_u, const float rnd_v) {
	pdf = 1.0f / (4.0f*PI);
	float z = rnd_u*2.0f - 1.0f;
	float r = sqrt(max(.0f, 1.0f - z*z));
	float phi = rnd_v*2.0f*PI;
	return vec(r*cos(phi),
		r*sin(phi),
		z);
}

class mat {
public:
	float m[4][4];
	mat() {
		m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] =
			m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = .0f;
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	}
	mat(const float other[4][4]) { memcpy(m, other, 16 * sizeof(float)); }
	mat(const float a) { for (int i = 0; i<4; ++i) for (int j = 0; j<4; ++j) m[i][j] = a; }
	mat(const mat& other) { memcpy(m, other.m, 16 * sizeof(float)); }
	mat(const float& m00, const float& m01, const float& m02, const float& m03,
		const float& m10, const float& m11, const float& m12, const float& m13,
		const float& m20, const float& m21, const float& m22, const float& m23,
		const float& m30, const float& m31, const float& m32, const float& m33) {
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}
	mat T() const {
		return mat(m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}
	bool operator==(const mat& other) {
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				if (m[i][j] != other.m[i][j])
					return false;
		return true;
	}
	bool operator!=(const mat& other) {
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				if (m[i][j] != other.m[i][j])
					return true;
		return false;
	}
	float operator[](const int i) const {
		return m[i / 4][i % 4];
	}
	float & operator[](const int i) {
		return m[i / 4][i % 4];
	}
	mat operator+(const mat& other) const {
		mat ret(.0f);
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				ret.m[i][j] = m[i][j] + other.m[i][j];
		return ret;
	}
	mat & operator+=(const mat& other) {
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				m[i][j] += other.m[i][j];
		return *this;
	}
	mat operator*(const mat& other) const {
		mat ret(.0f);
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				for (int k = 0; k<4; ++k)
					ret.m[i][j] += m[i][k] * other.m[k][j];
		return ret;
	}
	mat & operator*=(const mat& other) {
		mat ret(.0f);
		for (int i = 0; i<4; ++i)
			for (int j = 0; j<4; ++j)
				for (int k = 0; k<4; ++k)
					ret.m[i][j] += m[i][k] * other.m[k][j];
		memcpy((void*)m, ret.m, sizeof(float) * 16);
		return *this;
	}
	mat operator*(const float v) const {
		return mat(v*m[0][0], v*m[0][1], v*m[0][2], v*m[0][3],
			v*m[1][0], v*m[1][1], v*m[1][2], v*m[1][3],
			v*m[2][0], v*m[2][1], v*m[2][2], v*m[2][3],
			v*m[3][0], v*m[3][1], v*m[3][2], v*m[3][3]);
	}
	mat & operator*=(const float v) {
		m[0][0] *= v; m[0][0] *= v; m[0][0] *= v; m[0][0] *= v;
		m[0][0] *= v; m[0][0] *= v; m[0][0] *= v; m[0][0] *= v;
		m[0][0] *= v; m[0][0] *= v; m[0][0] *= v; m[0][0] *= v;
		m[0][0] *= v; m[0][0] *= v; m[0][0] *= v; m[0][0] *= v;
		return *this;
	}
	mat operator/(const float v) const {
		float iv = 1.0f / v;
		return mat(iv*m[0][0], iv*m[0][1], iv*m[0][2], iv*m[0][3],
			iv*m[1][0], iv*m[1][1], iv*m[1][2], iv*m[1][3],
			iv*m[2][0], iv*m[2][1], iv*m[2][2], iv*m[2][3],
			iv*m[3][0], iv*m[3][1], iv*m[3][2], iv*m[3][3]);
	}
	mat & operator/=(const float v) {
		float iv = 1.0f / v;
		m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv;
		m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv;
		m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv;
		m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv; m[0][0] *= iv;
		return *this;
	}
	float det() const {
		return  m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[1][2] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) + m[1][3] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]))
			- m[0][1] * (m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[1][2] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[1][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]))
			+ m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) - m[1][1] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]))
			- m[0][3] * (m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) - m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) + m[1][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));
	}
	mat inv() const {
		float d = det();
		if (d == .0f) {
			std::cout << std::endl << "Trying to invert a singular matrix!" << std::endl;
			return mat();
		}
		//this has got to be the coolest return statement i've ever written
		return mat(m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] - m[1][3] * m[2][2] * m[3][1],
			m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] + m[0][3] * m[2][2] * m[3][1] - m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2],
			m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] - m[0][3] * m[1][2] * m[3][1],
			m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1] - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2],
			m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] + m[1][3] * m[2][2] * m[3][0] - m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2],
			m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] - m[0][3] * m[2][2] * m[3][0],
			m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] + m[0][3] * m[1][2] * m[3][0] - m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2],
			m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0],
			m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] - m[1][3] * m[2][1] * m[3][0],
			m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] + m[0][3] * m[2][1] * m[3][0] - m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1],
			m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] - m[0][3] * m[1][1] * m[3][0],
			m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0] - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1],
			m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] + m[1][2] * m[2][1] * m[3][0] - m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1],
			m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] - m[0][2] * m[2][1] * m[3][0],
			m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] + m[0][2] * m[1][1] * m[3][0] - m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1],
			m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0]) / d;
	}
	mat & invert() {
		*this = inv();
		return *this;
	}
};

inline mat operator*(const float& v, const mat& m) {
	return m*v;
}

inline std::ostream& operator<<(std::ostream& out, const mat& a) {
	out << std::endl << "{ " << a.m[0][0] << ", " << a.m[0][1] << ", " << a.m[0][2] << ", " << a.m[0][3] << " }";
	out << std::endl << "{ " << a.m[1][0] << ", " << a.m[1][1] << ", " << a.m[1][2] << ", " << a.m[1][3] << " }";
	out << std::endl << "{ " << a.m[2][0] << ", " << a.m[2][1] << ", " << a.m[2][2] << ", " << a.m[2][3] << " }";
	out << std::endl << "{ " << a.m[3][0] << ", " << a.m[3][1] << ", " << a.m[3][2] << ", " << a.m[3][3] << " }";
	out << std::endl;
	return out;
}

class quat {
public:
	vec v; float w;
	quat() : v(), w() { }
	//make sure axis is normalized
	quat(const float ang, const vec axis) : v(axis*sin(ang*.5f)), w(cos(ang*.5f)) { }
	quat(const quat & other) : v(other.v), w(other.w) { }
	quat(const vec v, const float w) : v(v), w(w) { }
	quat(const float x, const float y, const float z, const float w) : v(x, y, z), w(w) { }

	quat operator+(const quat& other) const {
		return quat(v + other.v, w + other.w);
	}
	quat& operator+=(const quat& other) {
		v += other.v;
		w += other.w;
		return *this;
	}
	quat operator-() const {
		return quat(-v, -w);
	}
	quat operator-(const quat& other) const {
		return quat(v - other.v, w - other.w);
	}
	quat& operator-=(const quat& other) {
		v -= other.v;
		w -= other.w;
		return *this;
	}
	quat operator*(const quat& other) const {
		return quat(cross(v, other.v) + w*other.v + other.w*v, w*other.w - dot(v, other.v));
	}
	quat operator*(const float a) const {
		return quat(v*a, w*a);
	}
	quat& operator-=(const float a) {
		v *= a;
		w *= a;
		return *this;
	}
	quat operator/(const float a) const {
		float ia = 1.0f / a;
		return quat(v*ia, w*ia);
	}
	quat& operator/=(const float a) {
		float ia = 1.0f / a;
		v *= ia;
		w *= ia;
		return *this;
	}
	mat matrix() const {
		mat r;
		r.m[0][0] = 1.0f - 2.0f*(v.y*v.y + v.z*v.z);
		r.m[1][0] = 2.0f*(v.x*v.y - v.z*w);
		r.m[2][0] = 2.0f*(v.x*v.z + v.y*w);

		r.m[0][1] = 2.0f*(v.x*v.y + v.z*w);
		r.m[1][1] = 1.0f - 2.0f*(v.x*v.x + v.z*v.z);
		r.m[2][1] = 2.0f*(v.y*v.z - v.x*w);

		r.m[0][2] = 2.0f*(v.x*v.z - v.y*w);
		r.m[1][2] = 2.0f*(v.y*v.z + v.x*w);
		r.m[2][2] = 1.0f - 2.0f*(v.x*v.x + v.y*v.y);

		return r;
	}
};

inline quat operator*(const float a, const quat q) {
	return q*a;
}

inline float dot(const quat& a, const quat& b) {
	return dot(a.v, b.v) + a.w*b.w;
}

inline quat normalize(const quat& a) {
	return a / dot(a, a);
}

inline quat slerp(const quat& a, const quat& b, float t) {
	float costh = dot(a, b);
	if (costh>.9997f) return normalize((1.0f - t)*a + t*b);
	else {
		if (costh<-1.0f) costh = -1.0f;
		if (costh>1.0f) costh = 1.0f;
		float th_t = t * acos(costh); // current theta
		quat perp = normalize(b - a*costh);
		return a*cos(th_t) + perp*sin(th_t);
	}
}

class trf {
public:
	mat m, inv;

	trf() : m(), inv() { }
	trf(const mat& m, const mat& inv) : m(m), inv(inv) { }
	trf(const mat& m) : m(m), inv(m.inv()) { }
	trf(const vec& a, const vec& b, const vec& c, const vec d = vec(.0f)) {
		m.m[0][0] = a.x; m.m[0][1] = a.y; m.m[0][2] = a.z; m.m[0][3] = d.x;
		m.m[1][0] = b.x; m.m[1][1] = b.y; m.m[1][2] = b.z; m.m[1][3] = d.y;
		m.m[2][0] = c.x; m.m[2][1] = c.y; m.m[2][2] = c.z; m.m[2][3] = d.z;
		m.m[3][0] = .0f; m.m[3][1] = .0f; m.m[3][2] = .0f; m.m[3][3] = 1.0f;
		inv = m.inv();
	}

	trf inverse() {
		return trf(inv, m);
	}

	trf T() {
		return trf(m.T(), inv.T());
	}

	trf operator*(const trf& other) const {
		return trf(m*other.m, other.inv*inv);
	}

	trf& operator*=(const trf& other) {
		m *= other.m;
		inv = other.inv*inv;
		return *this;
	}

	inline vec operator()(const vec& v) const {
		return vec(v.x*m.m[0][0] + v.y*m.m[0][1] + v.z*m.m[0][2],
			v.x*m.m[1][0] + v.y*m.m[1][1] + v.z*m.m[1][2],
			v.x*m.m[2][0] + v.y*m.m[2][1] + v.z*m.m[2][2]);
	}
	inline void operator()(const vec& v, vec& r) const {
		float x = v.x, y = v.y, z = v.z;
		r.x = x*m.m[0][0] + y*m.m[0][1] + z*m.m[0][2];
		r.y = x*m.m[1][0] + y*m.m[1][1] + z*m.m[1][2];
		r.z = x*m.m[2][0] + y*m.m[2][1] + z*m.m[2][2];
	}
	inline pos operator()(const pos& p) const {
		pos r = pos(p.x*m.m[0][0] + p.y*m.m[0][1] + p.z*m.m[0][2] + m.m[0][3],
			p.x*m.m[1][0] + p.y*m.m[1][1] + p.z*m.m[1][2] + m.m[1][3],
			p.x*m.m[2][0] + p.y*m.m[2][1] + p.z*m.m[2][2] + m.m[2][3]);
		float w = p.x*m.m[3][0] + p.y*m.m[3][1] + p.z*m.m[3][2] + m.m[3][3];
		if (w != 1.0) r /= w;
		return r;
	}
	inline pos operator()(const pos& p, vec& r) const {
		float x = p.x, y = p.y, z = p.z;
		r.x = x*m.m[0][0] + y*m.m[0][1] + z*m.m[0][2] + m.m[0][3];
		r.y = x*m.m[1][0] + y*m.m[1][1] + z*m.m[1][2] + m.m[1][3];
		r.z = x*m.m[2][0] + y*m.m[2][1] + z*m.m[2][2] + m.m[2][3];
		float w = x*m.m[3][0] + y*m.m[3][1] + z*m.m[3][2] + m.m[3][3];
		if (w != 1.0) r /= w;
	}
	inline nor operator()(const nor& v) const {
		return nor(v.x*inv.m[0][0] + v.y*inv.m[1][0] + v.z*inv.m[2][0],
			v.x*inv.m[0][1] + v.y*inv.m[1][1] + v.z*inv.m[2][1],
			v.x*inv.m[0][2] + v.y*inv.m[1][2] + v.z*inv.m[2][2]);
	}
	inline void operator()(const nor& v, nor& r) const {
		float x = v.x, y = v.y, z = v.z;
		r.x = x*inv.m[0][0] + y*inv.m[1][0] + z*inv.m[2][0];
		r.y = x*inv.m[0][1] + y*inv.m[1][1] + z*inv.m[2][1];
		r.z = x*inv.m[0][2] + y*inv.m[1][2] + z*inv.m[2][2];
	}

	inline vec invt(const vec& v) const {
		return vec(v.x*inv.m[0][0] + v.y*inv.m[0][1] + v.z*inv.m[0][2],
			v.x*inv.m[1][0] + v.y*inv.m[1][1] + v.z*inv.m[1][2],
			v.x*inv.m[2][0] + v.y*inv.m[2][1] + v.z*inv.m[2][2]);
	}
	inline void invt(const vec& v, vec& r) const {
		float x = v.x, y = v.y, z = v.z;
		r.x = x*inv.m[0][0] + y*inv.m[0][1] + z*inv.m[0][2];
		r.y = x*inv.m[1][0] + y*inv.m[1][1] + z*inv.m[1][2];
		r.z = x*inv.m[2][0] + y*inv.m[2][1] + z*inv.m[2][2];
	}
	inline pos invt(const pos& p) const {
		pos r = pos(p.x*inv.m[0][0] + p.y*inv.m[0][1] + p.z*inv.m[0][2] + inv.m[0][3],
			p.x*inv.m[1][0] + p.y*inv.m[1][1] + p.z*inv.m[1][2] + inv.m[1][3],
			p.x*inv.m[2][0] + p.y*inv.m[2][1] + p.z*inv.m[2][2] + inv.m[2][3]);
		float w = p.x*inv.m[3][0] + p.y*inv.m[3][1] + p.z*inv.m[3][2] + inv.m[3][3];
		if (w != 1.0) r /= w;
		return r;
	}
	inline pos invt(const pos& p, vec& r) const {
		float x = p.x, y = p.y, z = p.z;
		r.x = x*inv.m[0][0] + y*inv.m[0][1] + z*inv.m[0][2] + inv.m[0][3];
		r.y = x*inv.m[1][0] + y*inv.m[1][1] + z*inv.m[1][2] + inv.m[1][3];
		r.z = x*inv.m[2][0] + y*inv.m[2][1] + z*inv.m[2][2] + inv.m[2][3];
		float w = x*inv.m[3][0] + y*inv.m[3][1] + z*inv.m[3][2] + inv.m[3][3];
		if (w != 1.0) r /= w;
	}
	inline nor invt(const nor& v) const {
		return nor(v.x*m.m[0][0] + v.y*m.m[1][0] + v.z*m.m[2][0],
			v.x*m.m[0][1] + v.y*m.m[1][1] + v.z*m.m[2][1],
			v.x*m.m[0][2] + v.y*m.m[1][2] + v.z*m.m[2][2]);
	}
	inline void invt(const nor& v, nor& r) const {
		float x = v.x, y = v.y, z = v.z;
		r.x = x*m.m[0][0] + y*m.m[1][0] + z*m.m[2][0];
		r.y = x*m.m[0][1] + y*m.m[1][1] + z*m.m[2][1];
		r.z = x*m.m[0][2] + y*m.m[1][2] + z*m.m[2][2];
	}

	//make sure axis is normalized
	static trf rotate(const float ang, const vec& axis) {
		trf r;
		r.m = normalize(quat(ang, axis)).matrix();
		r.inv = r.m.T();
		return r;
	}
	static trf rotate(const quat& q) {
		trf r;
		r.m = q.matrix();
		r.inv = r.m.T();
		return r;
	}
	static trf translate(float x, float y, float z) {
		trf r;
		r.m.m[0][3] = x; r.m.m[1][3] = y; r.m.m[2][3] = z;
		r.inv.m[0][3] = -x; r.inv.m[1][3] = -y; r.inv.m[2][3] = -z;
		return r;
	}
	static trf translate(pos p) {
		return translate(p.x, p.y, p.z);
	}
	static trf scale(float x, float y, float z) {
		trf r;
		r.m.m[0][0] = x;
		r.m.m[1][1] = y;
		r.m.m[2][2] = z;

		r.inv.m[0][0] = 1.0f / x;
		r.inv.m[1][1] = 1.0f / y;
		r.inv.m[2][2] = 1.0f / z;

		return r;
	}
	//up must be normalized!
	static trf lookat(const vec& origin, const vec& target, const vec& up) {
		vec dir = normalize(target - origin);
		vec right = normalize(cross(dir, up));
		vec corr_up = cross(right, dir);
		mat m;
		m.m[0][0] = right.x;
		m.m[1][0] = right.y;
		m.m[2][0] = right.z;

		m.m[0][1] = corr_up.x;
		m.m[1][1] = corr_up.y;
		m.m[2][1] = corr_up.z;

		m.m[0][2] = dir.x;
		m.m[1][2] = dir.y;
		m.m[2][2] = dir.z;

		m.m[0][3] = origin.x;
		m.m[1][3] = origin.y;
		m.m[2][3] = origin.z;
		return trf(m);
	}
	static trf orthographic(const float aspect, const float p_scale, const float znear, const float zfar) {
		return scale(p_scale*aspect, p_scale, 1.0f / (zfar - znear))*translate(.0f, .0f, -znear);
	}
	static trf perspective(const float aspect, const float fov, const float znear, const float zfar) {
		float f_fn = zfar / (zfar - znear);
		trf perspective_(mat(1.0f, .0f, .0f, .0f,
			.0f, 1.0f, .0f, .0f,
			.0f, .0f, f_fn, -znear*f_fn,
			.0f, .0f, 1.0f, .0f));
		float itan = 1.0f / tan(fov / 2.0f);
		return scale(itan*aspect, itan, 1.0f)*perspective_;
	}
};
