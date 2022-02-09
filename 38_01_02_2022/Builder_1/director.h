#pragma once

class Builder;

class Director {
	Builder *m_pbuilder ;
public:
	Director(Builder* builder);
	void construct();
};

