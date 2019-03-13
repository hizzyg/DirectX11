#pragma once

XMFLOAT2 operator+(XMFLOAT2 _lv, XMFLOAT2 _rv);
XMFLOAT2 operator-(XMFLOAT2 _lv, XMFLOAT2 _rv);

XMFLOAT2 operator*(XMFLOAT2 _lv, float _rv);
XMFLOAT2 operator/(XMFLOAT2 _lv, float _rv);

XMFLOAT3 operator+(XMFLOAT3 _lv, XMFLOAT3 _rv);
XMFLOAT3 operator-(XMFLOAT3 _lv, XMFLOAT3 _rv);
						  
XMFLOAT3 operator*(XMFLOAT3 _lv, float _rv);
XMFLOAT3 operator/(XMFLOAT3 _lv, float _rv);

XMFLOAT4 operator+(XMFLOAT4 _lv, XMFLOAT4 _rv);
XMFLOAT4 operator-(XMFLOAT4 _lv, XMFLOAT4 _rv);

XMFLOAT4 operator*(XMFLOAT4 _lv, float _rv);
XMFLOAT4 operator/(XMFLOAT4 _lv, float _rv);

float Clamp(float _value, float _min, float _max);
XMFLOAT2 Clamp(XMFLOAT2 _value, XMFLOAT2 _min, XMFLOAT2 _max);
XMFLOAT3 Clamp(XMFLOAT3 _value, XMFLOAT3 _min, XMFLOAT3 _max);

float Magnitude(XMFLOAT2 _input);
float Magnitude(XMFLOAT3 _input);

XMFLOAT2 Normalize(XMFLOAT2 _input);
XMFLOAT3 Normalize(XMFLOAT3 _input);

bool PointInRect(XMFLOAT2 _pos, XMFLOAT2 _extends, XMFLOAT2 _point);

XMFLOAT2 Lerp(XMFLOAT2 _a, XMFLOAT2 _b, float _t); 
XMFLOAT3 Lerp(XMFLOAT3 _a, XMFLOAT3 _b, float _t); 
XMFLOAT4 Lerp(XMFLOAT4 _a, XMFLOAT4 _b, float _t);