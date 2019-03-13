#include "PCH.h"
#include "Helper.h"

XMFLOAT2 operator+(XMFLOAT2 _lv, XMFLOAT2 _rv)
{
	return XMFLOAT2(_lv.x + _rv.x, _lv.y + _rv.y);
}

XMFLOAT2 operator-(XMFLOAT2 _lv, XMFLOAT2 _rv)
{
	return XMFLOAT2(_lv.x - _rv.x, _lv.y - _rv.y);
}

XMFLOAT2 operator*(XMFLOAT2 _lv, float _rv)
{
	return XMFLOAT2(_lv.x * _rv, _lv.y * _rv);
}

XMFLOAT2 operator/(XMFLOAT2 _lv, float _rv)
{
	return XMFLOAT2(_lv.x / _rv, _lv.y / _rv);
}

XMFLOAT3 operator+(XMFLOAT3 _lv, XMFLOAT3 _rv)
{
	return XMFLOAT3(_lv.x + _rv.x, _lv.y + _rv.y, _lv.z + _rv.z);
}

XMFLOAT3 operator-(XMFLOAT3 _lv, XMFLOAT3 _rv)
{
	return XMFLOAT3(_lv.x - _rv.x, _lv.y - _rv.y, _lv.z - _rv.z);
}

XMFLOAT3 operator*(XMFLOAT3 _lv, float _rv)
{
	return XMFLOAT3(_lv.x * _rv, _lv.y * _rv, _lv.z * _rv);
}

XMFLOAT3 operator/(XMFLOAT3 _lv, float _rv)
{
	return XMFLOAT3(_lv.x / _rv, _lv.y / _rv, _lv.z / _rv);
}

XMFLOAT4 operator+(XMFLOAT4 _lv, XMFLOAT4 _rv)
{
	return XMFLOAT4(_lv.x + _rv.x, _lv.y + _rv.y, _lv.z + _rv.z, _lv.w + _rv.w);
}

XMFLOAT4 operator-(XMFLOAT4 _lv, XMFLOAT4 _rv)
{
	return XMFLOAT4(_lv.x - _rv.x, _lv.y - _rv.y, _lv.z - _rv.z, _lv.w - _rv.w);
}

XMFLOAT4 operator*(XMFLOAT4 _lv, float _rv)
{
	return XMFLOAT4(_lv.x * _rv, _lv.y * _rv, _lv.z * _rv, _lv.w * _rv);
}

XMFLOAT4 operator/(XMFLOAT4 _lv, float _rv)
{
	return XMFLOAT4(_lv.x / _rv, _lv.y / _rv, _lv.z / _rv, _lv.w / _rv);
}

float Clamp(float _value, float _min, float _max)
{
	return _value < _min
		? _min
		: _value > _max
		? _max
		: _value;
}

XMFLOAT2 Clamp(XMFLOAT2 _value, XMFLOAT2 _min, XMFLOAT2 _max)
{
	return XMFLOAT2(Clamp(_value.x, _min.x, _max.x),
		Clamp(_value.y, _min.y, _max.y));
}

XMFLOAT3 Clamp(XMFLOAT3 _value, XMFLOAT3 _min, XMFLOAT3 _max)
{
	return XMFLOAT3(Clamp(_value.x, _min.x, _max.x),
		Clamp(_value.y, _min.y, _max.y),
		Clamp(_value.z, _min.z, _max.z));
}

float Magnitude(XMFLOAT2 _input)
{
	return sqrtf(_input.x * _input.x + _input.y * _input.y);
}

float Magnitude(XMFLOAT3 _input)
{
	return sqrtf(_input.x * _input.x + _input.y * _input.y + _input.z * _input.z);
}

XMFLOAT2 Normalize(XMFLOAT2 _input)
{
	return _input / Magnitude(_input);
}

XMFLOAT3 Normalize(XMFLOAT3 _input)
{
	return _input / Magnitude(_input);
}

bool PointInRect(XMFLOAT2 _pos, XMFLOAT2 _extends, XMFLOAT2 _point)
{
	return (_point.x >= _pos.x && _point.x <= _pos.x + _extends.x)
		&& (_point.y >= _pos.y && _point.y <= _pos.y + _extends.y);
}

XMFLOAT2 Lerp(XMFLOAT2 _a, XMFLOAT2 _b, float _t)
{
	return _a + (_b - _a) * _t;
}

XMFLOAT3 Lerp(XMFLOAT3 _a, XMFLOAT3 _b, float _t)
{
	return _a + (_b - _a) * _t;
}

XMFLOAT4 Lerp(XMFLOAT4 _a, XMFLOAT4 _b, float _t)
{
	return _a + (_b - _a) * _t;
}
