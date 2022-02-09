#pragma once

class Builder
{
public:
	virtual ~Builder() = default;
	virtual void build_part_a() = 0;
	virtual void build_part_b() = 0;
	virtual void build_part_c() = 0;
};

