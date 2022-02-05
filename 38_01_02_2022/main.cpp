/*

FACTORY METHODDA KALMIŞTIK
Burada client kod arka tarafta hangi sınıfın kullanıldığınıda bilmiyor.
Bu fabrika metodu kalıplarının görüntüsü farklı bile olsa hepsi runtimeda oluşturulacak nesne
yada nesnelerin türünün belirlenmesiyle ilgili. Eğer bir tane nesnenin türünün belirlenmesinden
bahsediyorsak o zaman design pattern factory method oluyor ama bir ürün ailesinin oluşturulması söz konusu ise
o zaman abstract factory devreye giriyor.

--------------------------------------------------------------------------------------------

Buradaki örneği githuba ayrı klasöre koyuyorum
Folder adı : FACTORIES_2

--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------

ABSTRACT FACTORY
----------------
TÜm mantık aynı değişen şey bir ürün oluşturmak yerine ürün ailesi oluşturmak.
Bunlar mantıksal ilişki içerisinde.

Tanım : Birbiriyle bağlı veya ilişkili bir ürün ailesi oluşturulacak. 
Ama Bunların somut sınıflarının ne olduğu bilinmeden yapılacak.

NOT : Polymorphic sınıfların destructorları virtual olmalı.


Abstract Factory
----------------
Ürün grubu oluşturmak için bir interface declere ediyor


Concrete Factory
----------------
Abstract factorynin oluşturduğu interfacei implemente ediyor.
Yani pure virtual funcları override ediyor.


Abstract Product
----------------
product objesi için bir interface declere ediyor.


Concrete Product
----------------
Örnekte meseala html_document.h bir concrete product.


//////////////////
abstract factory ==> gui_factory


buradan kalıtımla elde edilenlerin her biri concrete factory
windows_gui_factory
linux_gui_factory
ios_gui_factory


bunlar abstract product
slider/menu/textbox/checjbox/button


bunlardan kalıtımla elde edilen sınıflar concrete product
Reply
Copy Selected Text
Pin
Forward
Select
Delete


abstract factory kullanarak bir ürün kitini oluşturuyoruz


ama nasıl bir ürün kiti kullanılacağını belirleyen seçilen concrete factory sınıfı oluyor

//////////////////

EXAMPLE2 yapıldı githuba yüklendi.

--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------

BİRKAÇ TANE İDİYOM GÖSTERİLECEK
-------------------------------

RETURN TYPE RESOLVER
--------------------
Diğer dillerde hazır gelen bir araç.

#include <vector>
#include <list>
#include <set>

int main()
{
	std::vector<int>ivec = range(12,45);
	std::list<int>ilist = range(34,45);
}

İster vector istersemde list oluşturabiliyoruz
Strateji aşağıdaki gibi
Range bir sınıf ve biz yazacağız. Range buradaki verileri alıp bir veri elemanında tutacak.
Bir conversion func koyacağız operator funcı.
Buradaki nesne compile timede vector<int> olduğu görülürse, tür dönüştürme operator func template olarak yazılacak.
Bu durumda derleyici durumdan vazife çıkartıp operator vector<int> funcını yazacak. Derleyici diğer durumda da operator list<int> yazacak.
Hatta nullptr de böyle implemente ediliyor. Onuda sağlayan return type resolver

--------------------------------------------------------------------------------------------

Range Sınıfını Yazıyoruz
------------------------

#include<stdexcept>

class Range{
public:
	Range(int from, int to) : m_from{from}, m_to{to} 
	{
		if(from > to)
		{
			throw std::runtime_error{"invalid range values"};
		}
	}

	template<typename Con>
	operator Con()const
	{
		Con con;
		for(int i{m_from}; i < m_to; ++i)
		{
			con.insert(con.end(), i); // multiset set map ... associative containerların insert funcı konum istemez.Ama onlar çin yinede iterator parametreli func var
		}								// ama iterator parametreli insert funcı setin mesela, oraya insert etmiyor. Orayı bir Hint olarak alıyor.Böylece bir verim artışı oluyor.
		return con;
	}

private:
	int m_from, m_to;
};



#include<vector>
#include<list>
#include<set>
#include<iostream>

int main()
{
	std::list<int> mylist = Range{10,45}; // template buradaki conversionu yaptı.
	
	for(auto val : mylist)
		std::cout << val << ' ';

	std::cout << "\n\n";

	// --------------------------------------------------------

	std::set<int> myset = Range{10,45}; // template buradaki conversionu yaptı.

	for(auto val : myset)
		std::cout << val << ' ';

	std::cout << "\n\n";

	// --------------------------------------------------------

	std::vector<int> myvec = Range{777,755}; // template buradaki conversionu yaptı.

	for(auto val : myvec)
		std::cout << val << ' ';

}

-------------------------------------------------------------------------------------------

STRINGIN TAMSAYIYA YADA GERÇEK SAYIYA OTOMATİK OLARAK DÖNÜŞMESİNİ İSTİYORUZ DİYELİM

Containment yoluyla string nesnesini alıp kullanabiliriz
Containment yoluyla composition en çok adaptörlerde kullanılıyor.
stack queue priority queue.Bunlar container adapters

int main()
{
	using namespace std;

	string str{"1247345"};
	int ival = str;
}


class Nstring{
public:
	Nstring(const std::string &str) : m_str{str} { }
	operator int()const{return std::stoi(m_str); }
	operator double()const{return std::stod(m_str); }

private:
	std::string m_str;
};

int main()
{
	int ival = Nstring{"765123"};
	int dval = Nstring{"765.123"};

}

-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------


NULLPTR İÇİNDE AYNISI YAPILIYOR.
Nullptr olmadan function overloading konusunda problem oluşuyor.
örneğin nullptr olarak 0 sabiti kullanılıyordu. 0, C ve C++ ta nullptr a dönüşüyor.
0 herşeye dönüşüyor.Sadece pointer değil.

void func(int *);
void func(int);

int main()
{
	func(0); // in parametreli çağrılır.nullptr eklenmesi çok iyi oldu.
}

-------------------------------------------------------------------------------------------

NULLPTR Yİ AŞAĞIDAKİ GİBİ DÜŞÜNEBİLİRİZ

constexpr class null_ptr_t
{
public:
	template <typename T>
	constexpr operatorT*()const
	{
		return 0;
	}

	template <typename C, typename T>
	operator T C::*()const
	{
		return 0;
	}

	void operator&() = delete;

}null_ptr;


//Burada da yukarıdaki overloadlar var
void func(int)
{
	std::cout << "func(int)\n";
}

void func(int *)
{
	std::cout << "func(int *)\n";
}

int main()
{
	int x = null_ptr;	//sentaks hatası
	int *p = null_ptr;	//Geçerli

	func(0); // func(int) çağrılır
	func(null_ptr); // func(int *) çağrılır

}

-------------------------------------------------------------------------------------------

::* IÇINDE ÖRNEK 

class Myclass{
public:
	int func(int);
};

int main()
{
	auto fptr = &Myclass::func;
	fptr = nullptr; // Valid
}


-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------

NAMED PARAMETER/ARGUMENT

C++ a eklemiyorlar bunu.Bu bir eksiklik.
Sadece positional argument var. 1. parametreye 1. argüman, 2. parametreye 2. argüman gibi.

Özellikle ctor tarafında parametre sayısının fazla olması çok tipik bir durum.Windows apilerinde çok var bu.
Burada en önemli yardımcılardan biri default parametre.

-------------------------------------------------------------------------------------------

class File{
public:
	File(std::string name, bool read_only, bool archive, unsigned int block_size);

};

int main()
{
	File myfile("necati.txt",true,false,true,2345); // çağrıda anlamsız.
}

-------------------------------------------------------------------------------------------

NAMED PARAMETER BAZI TEKNIKLER ILE YAPILIYOR.


class Openfile{
public:
	OpenFile(const std::string& filename) : m_file_name{filename} {  }
	OpenFile& readonly()
	{
		m_read_only = true;
		return *this;
	}

	OpenFile& read_write()
	{
		m_read_only = false;
		return *this;
	}
	
	OpenFile& block_size(std::size_t size)
	{
		m_block_size= size;
		return *this;
	}
	
	OpenFile& create_if_not_exists(std::size_t size)
	{
		m_create_if_not_exists = true;
		return *this;
	}
private:
	friend class File;
	std::string m_file_name;
	bool m_read_only;
	bool m_create_if_not_exists;
	std::size_t m_block{4096};
};


class File{
public:
	File(const OpenFile &params) : m_file_name{params.m_file_name}, 
	m_read_only{params.m_read_only},
	m_create_if_not_exists{params.m_create_if_not_exists},
	m_block_size{params.m_block_size} {  }

private:
	std::string m_file_name;
	bool m_read_only;
	bool m_create_if_not_exists;
	std::size_t m_block;
};


int main()
{
	File myfile(OpenFile("necati.dat").read_write().block_size(1024).create_if_not_exists()); //chaining yaptık.
			
}

Bu yapı duruma göre değer.Maliyet açısından bakınca burada inline expansion yaptığından ilave maliyet çok fazla getirmiyor.

--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------

BUILDER
-------
Nesnenin constructionını onun sunumundan/temsilinden ayırıyoruz ve aynı construction processi farklı farklı nesneler oluşturabiliyor.

1:55

#include <string>
#include <memory>

class Car{
public:
	Car(std::string color, double length, double witdh, bool sunroof_flag,int door_count);

	class Builder{ //nested type
	public:
		using Carptr = std::unique_ptr<Car>;
		CarPtr create()
		{
			return std::make_unique<Car>(m_color,m_length,m_width,m_has_sunroof, m_door_count);
		}
		Builder& set_color(std::string color)
		{
			m_color = std::move(color);
			return *this;
		}
		Builder& set_length(double length)
		{
			m_length = length;
			return *this;
		}
		Builder& set_width(double width)
		{
			m_width = width;
			return *this;
		}
		Builder& door_count(int count)
		{
			m_door_count = count;
			return *this;
		}
		Builder& has_sunroof()
		{
			m_has_sunroof = true;
			return *this;
		}
	private:
		std::string m_color{"white"};
		double m_length;
		double m_width;
		bool m_has_sunroof{};
		int m_door_count{4};
	};
private:
	std::string m_color;
	double m_length;
	double m_width;
	bool m_has_sunroof;
	int m_door_count;
	//..
};



int main()
{
	auto ptr = Car::Builder{}
	.set_color("Blue")
	.set_length(243.92)
	.set_width(234.75)
	.door_count(4)
	.has_sunroof()
	.create();
	
}






































































*/