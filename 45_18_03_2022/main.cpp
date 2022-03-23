/*

Double dispatch örneğini anlatılıyor.
Aslında visiter paternini göreceğiz, zaten double dispatch kullanılıyor burada da.

Double dispatch te davranış birden fazla nesnenin dinamik türüne göre belirleniyor.
Farklı programlama dillerinin farklı araçları var.Multimetod olabilir önerisi C++ ta 
verilmiş ama reddedilmiş, function overloading olabilir hatta burada overloading ile
overriding beraber kullanılıyor.

VISITOR
-------
Bir object structure var. Yani aynı interfacei destekleyen n tane nesnenin oluşturduğu bir 
küme var. Bunlar containerda tutuluyor olabilir.Recursive şekilde birbirini içeren şekilde de olabilir.
Amaç bu nesneler üzerinde yeni bir operasyon çalıştırmak.
Burada seçeneklerden biri tepedeki taban sınıfa bu interfacei eklemek ve bütün sınıflarında bunu override etmesini sağlamak.
Bunu 2-3 nedenle istemiyor yada yapamıyor olabiliriz.
1 - Mesela base class kodu bizde yoksa yapamayız.
2 - Sınıfın interfaceini böyle bir func ile kirletmiş olmak istemiyor olabiliriz.

GOFTAKİ TANIMI : Bir nesne kümesinin elemanlar üzerinde bir operasyonu çalıştırmak.
Bu sınıfları değiştirmeden yinede bu işlemleri yapabilmeyi sağlıyor.

NECATI ERGIN TANIMI : Ziyaretçi kalıbı algoritmaları, o algoritmaların kullanılacağı nesnelerden
ayırır. Sınıfları değiştirmeden o sınıflar türünden nesneler üzerinde çalıştırılacak yeni 
algoritmalar ekleyebiliyoruz. Farklı nesnelerden aggregate yapılara yeni bir davranış
eklemek istiyoruz. 
Bunların farklı arayüzleri var ve nesne yapısı nadiren değişiyor.


ÖR:

shape.h
-------
#pragma once
class ShapeVisitor;

class Shape{
public:
	//interface
	virtual ~Shape() = default;
	virtual std::string name()const = 0;
	virtual void accept(ShapeVisitor&) = 0; // Bunu sonradan ekledik.Aşağıdaki overridelarda sonradan eklendi.
	// ...
};


shape_visitor.h // sonradan eklendi
---------------
#pragma once

class Circle;
class Square;
class Rectangle;
class Triangle;

class ShapeVisitor  // Bu abstract sınıftan kalıtımla elde edilen sınıflar bu visitleri override edecek.
{
public:
	virtual void visit(Circle &) = 0;
	virtual void visit(Square &) = 0;
	virtual void visit(Rectangle &) = 0;
	virtual void visit(Triangle &) = 0;

};



// Bunların yüksekliklerinin hesaplanması ve hesaplanan bu değerler için ilgili object structure için
// tüm yüksekliklerinin toplanmasını yapacağız.
// Burada concrete visitor yazacağız.

height_visitor.h // SONRADAN EKLENDi
----------------
#pragma once
#include "shape_visitor.h"

class HeightVisitor : public ShapeVisitor
{
public:
	virtual void visit(Circle &)override;
	virtual void visit(Square &)override;
	virtual void visit(Rectangle &)override
	virtual void visit(Triangle &)override;

	double get_total_height()const {
		return m_total_height;
	}
private:
	double m_total_height{};
}


height_visitor.cpp // SONRADAN EKLENDI
------------------
#include "height_visitor.h"

#include "circle.h"
#include "square.h"
#include "rectangle.h"
#include "triangle.h"
#include <algorithm>

void HeightVisitor::visit(Circle &circle)
{
	m_total_height += circle.get_radius() * 2;	
}

void HeightVisitor::visit(Square &square)
{
	m_total_height += square.get_edge();	
}

void HeightVisitor::visit(Rectangle& rect)
{
	m_total_height += rect.get_long_edge();
}

void HeightVisitor::visit(Triangle& Tr)
{
	m_total_height += std::max({tr.get_edge1() , tr.get_edge1(), tr.get_edge1()}); // argüman olarak initializer list gönderdik maxa
}


EN SON EKLENEN KOD AREAVISITOR

area_visitor.h
--------------
#pragma once
#include "shape_visitor.h"

class AreaVisitor : public ShapeVisitor
{
public:
	virtual void visit(Circle &)override;
	virtual void visit(Square &)override;
	virtual void visit(Rectangle &)override
	virtual void visit(Triangle &)override;

	double total_area()const {
		return m_total_area;
	}
private:
	double m_total_area{};
}


area_visitor.cpp
----------------
#include "area_visitor.h"

#include "circle.h"
#include "square.h"
#include "rectangle.h"
#include "triangle.h"
#include <algorithm>

void AreaVisitor::visit(Circle &c)
{
	auto r = c.get_radius();
	m_total_area += 3.1415928 * r * r;
}

void AreaVisitor::visit(Square &s)
{
	auto edge = s.get_edge();
	m_total_area += 3.1415928 * edge * edge; 	
}

void AreaVisitor::visit(Rectangle& r)
{
	m_total_area += r.get_short_edge() * r.get_long_edge(); 	
	
}

void AreaVisitor::visit(Triangle& t)
{
	m_total_area += t.get_Edge_1() * t.get_edge_2() / 2;
}



circle.h
--------
#pragma once
#include "shape.h"
#include "shape_visitor.h"

class Circle : public Shape
{
public:
	Circle(double r) : m_radius{r} { }
	
	double get_radius()const { return m_radius;}
	
	virtual std::string name()const override
	{
		return "Circle";
	}

	virtual void accept(ShapeVisitor &visitor)override
	{
		visitor.visit(*this);
	}

private:
	double m_radius;
};


square.h
--------
#pragma once
#include "shape.h"
#include "shape_visitor.h"

class Square : public Shape
{
public:
	Square(double e) : m_edge{e} { }

	double get_radius()const {return m_edge;}

	double get_short_edge()const override
	{
		return "Square";
	}

	virtual void accept(ShapeVisitor &visitor)override
	{
		visitor.visit(*this);
	}

private:
	double m_edge;
};


rectangle.h
-----------
#pragma once
#include "shape.h"
#include "shape_visitor.h"

class Rectangle : public Shape
{
public:
	Rectangle(double e1, double e2) : m_edge1{e1}, m_edge2{e2} { }
	
	double get_short_edge()const 
	{ 
		return std::min(m_edge1, m_edge2);
	}
	
	double get_long_edge()const
	{
		return std::max(m_edge1, m_edge2);
	}

	virtual void accept(ShapeVisitor &visitor)override
	{
		visitor.visit(*this);
	}

private:
	double m_edge1;
	double m_edge2;
};


triangle.h
----------
#pragma once
#include "shape.h"
#include "shape_visitor.h"

class Triangle : public Shape
{
public:
	Triangle(double e1, double e2, double e3) : m_edge1{e1}, m_edge2{e2}, m_edge3{e3} { }

	double get_edge_1()const{ return m_edge1;}
	double get_edge_2()const{ return m_edge2;}
	double get_edge_3()const{ return m_edge3;}

	virtual std::string name()const override{ return "rectangle";}

	virtual void accept(ShapeVisitor &visitor)override
	{
		visitor.visit(*this);
	}

private:
	double m_edge1;
	double m_edge2;
	double m_edge3;
};



main.cpp
--------

#include "circle.h"
#include "square.h"
#include "rectangle.h"
#include "triangle.h"
#include <vector>
#include <iostream>
#include <random>
#include "height_visitor.h"

Shape* create_random_shape()
{
	static std::mt19937 eng{std::random_device{}()};
	static std::uniform_int_distribution idist{0, 3};
	static std::uniform_int_distribution rdist{0., 9.};

	switch(idist(eng))
	{
		case 0: return new Circle{rdist(eng)};
		case 1: return new Square{rdist(eng)};
		case 2: return new Rectangle{rdist(eng),rdist(eng)};
		case 3: return new Triangle{rdist(eng),rdist(eng)}; // Geçerli üçgen olmayabilir.Sorun değil bizim için.
	}

	return nullptr;
}

int main()
{
	using namespace std;

	vector<Shape*> svec;

	for(int i = 0; i < 1000; ++i)
	{
		svec.push_back(create_random_shape());
	}

	for(auto p : svec)
		std::cout << p->name() <<'\n';

	for(auto p : svec)
		delete p;

	// --------------------------------------------------------------

	//Yeni kodları ekledikten sonra yeni davranışa bakalım

	vector<Shape*> svec;

	for(int i = 0; i < 1000; ++i)
	{
		svec.push_back(create_random_shape());
	}

	HeightVisitor hv;

	for(auto p : svec)
		std::cout << p->accept(hv) <<'\n';

	std::cout << "Toplam yukseklik degeri : " << hv.get_total_height() << '\n';

	for(auto p : svec)
	delete p;
}


BU ÖRNEKTE HEIGHTVISITORU EKLEDIK. YANI YENI DAVRANIŞ ELDE ETMEK IÇIN KOD EKLEDIK.
BÖYLE BIRSÜRÜ VISITYOR YAZABILIRIZ. SINIFLARIN INTERAFACEINI KIRLETMEDIK.


===========================================================================================================================
===========================================================================================================================
===========================================================================================================================
===========================================================================================================================


ÖR:

petvisitor.h
------------
#pragma once

class Cat;
class Dog;
class Bird;

class PetVisitor{
public:
	virtual ~PetVisitor() = default;
	virtual void visit(Cat*) = 0;
	virtual void visit(Dog*) = 0;
	virtual void visit(Bird*) = 0;

};

playing_visitor.h
-----------------
#pragma once
#include "petvisitor.h"
#include <iostream>

class PlayingVisitor : public PetVisitor
{
public:
	void visit(Cat *)override;
	void visit(Dog *)override;
	void visit(Bird *)override;
};


playing_visitor.cpp
-------------------
#include "feeding_visitor.h"
#include <iostream>
#include <format>
#include "cat.h"
#include "dog.h"
#include "bird.h"

void PlayingVisitor::visit(Cat* cp)
{
	std::cout << std::format("{} isimli {} renkli kedi lazer fenerle oynatiliyor", cp->name(), cp->color()) << '\n';
}

void PlayingVisitor::visit(Dog* dp)
{
	std::cout << std::format("{} isimli {} renkli kopek top ile oynatiliyor", dp->name(), dp->color()) << '\n';
}

void PlayingVisitor::visit(Bird* bp)
{
	std::cout << std::format("{} isimli {} renkli kus ip ile oynatiliyor", bp->name(), bp->color()) << '\n';
}


feeding_visitor.h
-----------------
#pragma once
#include "petvisitor.h"
#include <iostream>

class FeedingVisitor : public PetVisitor
{
public:
	void visit(Cat *)override;
	void visit(Dog *)override;
	void visit(Bird *)override;
};

feeding_visitor.cpp
-------------------
#include "feeding_visitor.h"
#include <iostream>
#include <format>
#include "cat.h"
#include "dog.h"
#include "bird.h"

void FeedingVisitor::visit(Cat* cp)
{
	std::cout << std::format("{} isimli {} renkli kediye balik veriliyor", cp->name(),cp->color()) << '\n';
}

void FeedingVisitor::visit(Dog* cp)
{
	std::cout << std::format("{} isimli {} renkli kopege kemik veriliyor", cp->name(),cp->color()) << '\n';
}

void FeedingVisitor::visit(Bird* cp)
{
	std::cout << std::format("{} isimli {} renkli kusa yem veriliyor", cp->name(),cp->color()) << '\n';
}



pet.h
-----
#pragma once
#include <string>

class PetVisitor;

class Pet
{
public:
	virtual ~Pet() = default;
	virtual void accept(PetVisitor &v) = 0;
	Pet(const std::string& name, const std::string& color) : m_name{name}, m_color(color) {}
	const std::string& name()const { return m_name;}
	const std::string& color() const { return m_color;}
private:
	std::string m_name;
	std::string m_color;
};


bird.h
------
#pragma once
#include "pet.h"

class Bird : public Pet{
public:
	using Pet::Pet; // inherited ctor
	virtual void accept(PerVisitor&v)override;
};


bird.cpp
--------
#include "bird.h"
#include "petvisitor.h"

void Bird::accept(PetVisitor& v)
{
	v.visit(this);
}


cat.h
------
#pragma once
#include "pet.h"

class Cat : public Pet{
public:
	using Pet::Pet; // inherited ctor
	virtual void accept(PetVisitor&v)override;
};

cat.cpp
--------
#include "cat.h"
#include "petvisitor.h"

void Bird::accept(PetVisitor& v)
{
	v.visit(this);
}


dog.h
------
#pragma once
#include "pet.h"

class Dog : public Pet{
public:
	using Pet::Pet; // inherited ctor
	virtual void accept(PerVisitor&v)override;
};

dog.cpp
--------
#include "dog.h"
#include "petvisitor.h"

void Bird::accept(PetVisitor& v)
{
	v.visit(this);
}


//Bu barınak örneği.Barınağa hayvan ekleme işlemleri yapılıyor.
shelter.h
---------
#pragma once
#include <vector>
#include <memory>

class Pet;
class PetVisitor;

class Shelter{
public:
	void add(Pet*p);
	void accept(PetVisitor&p);
private:
	std::vector<std::unique_ptr<Pet>> m_pet_vec;
};

shelter.cpp
-----------
#include "shelter.h"
#include "pet.h"
#include "petvisitor.h"

void Shelter::Add(Pet *p)
{
	m_pet_vec.emplace_back(p);
}

void Shelter::accept(PetVisitor& v)
{
	for(auto &p : m_pet_vec)
	{
		p->accept(v);
	}
}



main.cpp
--------
#include "cat.h"
#include "dog.h"
#include "bird.h"
#include "feeding_visitor.h"
#include "playing_visitor.h"
#include "shelter.h"

void feed(Pet* pet, PetVisitor &visitor)
{
	pet->accept(visitor);
}

int main()
{
	Pet* p1 = new Cat("pamuk","beyaz");
	Pet* p1 = new Dog("kont","gri");
	Pet* p1 = new Bird("geveze","sari");

	FeedingVisitor fv;
	PlayingVisitor pv;

	p2->accept(fv);
	p3->accept(fv);
	p1->accept(pv);
	p2->accept(pv);
	p3->accept(pv);
	
	feed(p2,fv);
	feed(p2,pv);

	feed(p1,fv);
	feed(p1,pv);

	//----------------------------------------------
	// SHELTER İÇİN BAKIYORUZ

	Shelter s;
	s.add(new Cat("pamuk","beyaz"));
	s.add(new Dog("kont","gri"));
	s.add(new Bird("geveze","sari"));

	FeedingVisitor fv;
	s.accept(fv);

	PlayingVisitor pv;
	s.accept(pv);

}

===================================================================================================================
===================================================================================================================
===================================================================================================================
===================================================================================================================

SERIALIZATION
-------------
Hayatta olan nesnelerin değerlerini state bilgilerini dışarıya yazıyoruz(dosya veya bellek),
sonra bu dosyaya veya belleğe yazdığımız belirli bir formattaki texte dayanarak nesneyi tekrar 
oluşturabiliyoruz. Böylece aslında bir nevi nesneleri dosyalarda saklamış oluyoruz.
Serialization ve deserialization.

Bunlar doğrudan interfacee konulan funclar olmak yerine visitor olarak kullanılabilir.

VISITOR-2 Örneği hocadan al.

Burada visit hatırlatması yapıldı. Variant ile beraber kullanmıştık visiti.

===================================================================================================================
===================================================================================================================
===================================================================================================================
===================================================================================================================

ADAPTER VE DECORATOR 
--------------------

ADAPTER
-------
Tanımı yazmadım çünkü şekil gerekiyor ama daha önceden gördüğümüz container ve iterator adaptörleri
vardı. Container veya iterator alıp bunu implemente eden ve yeni interface sağlayan sınıflar.

ADAPTER-1 Hocadan al

2. versiyonunuda yaz.

ornek2.txt dosyasından bir örnek alındı onu iste


DECORATOR
---------
Bir nesneye ilave davranış kzandırmak için kullanılıyor.
Bir sınıfla ilgili değil bir nesne ile ilgili. Diğer nesneler bundan etkilenmiyor.

Bu ciddi bir problemi çözüyor. kalıtımdaki en büyük problemlerden biri inheritance explosion.
Yani çok sayıda sınıfın oluşması. her ilave özellik için kalıtımda yeni sınıf oluşuyor.
Her ilave davranışı sınıflara yüklersek ortada birsürü sınıf oluşur.


*/
