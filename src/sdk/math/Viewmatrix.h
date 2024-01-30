#pragma once

struct VMatrix 
{
	auto operator[](int i) { return m[i]; };

	float m[4][4];
};
