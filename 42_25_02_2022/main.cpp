/*

Geçen Ders NVI de kalmıştık oradan devam ediyoruz. yazılmış halini getirdi hoca.Ben br önceki derste yazmıştım.

#include <istream>
#include <ostream>
#include <cassert>
class Lock {
public:
	//...
	void acquire();
	void release();
};
class Data {
public:
	//...
	bool check_invariants()const;
};
class Base {
private:
	Lock m_lock;
	Data m_data;
public:
	void read(std::istream& iss)  //virtual değil
	{
		m_lock.acquire();
		assert(m_data.check_invariants()); // must be true
		do_read(iss);
		assert(m_data.check_invariants()); // must be true
		m_lock.release();
	}
	void write_to(std::ostream& o) // virtual değil
	{
		m_lock.acquire();
		do_write(o);
		m_lock.release();
	}
	virtual ~Base() {}
private:
	virtual void do_read(std::istream&) = 0;
	virtual void do_write(std::ostream&) = 0;
};
class XMLReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read XML.
	}
	void do_write(std::ostream&) override
	{
		// Write XML.
	}
};
class JasonReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read Jason.
	}
	void do_write(std::ostream&) override
	{
		// Write Jason.
	}
};
class TextReaderWriter : public Base {
private:
	void do_read(std::istream&) override
	{
		// Read Text
	}
	void do_write(std::ostream&) override
	{
		//write text
	}
};

--------------------------------------------------------------------------------------------------------------------

DİĞER ÖRNEK BENZER TEMA VAR YİNE

#include <iostream>

class Widget
{
public:
	void draw()
	{
		clear_background();
		paint();
	}
	virtual ~Widget() = default;
private:
	virtual void paint() = 0;
protected:
	virtual void clear_background()
	{
		std::cout << "clearing Widget background..." << '\n';
	}
};

class Button : public Widget
{
private:
	virtual void paint() override
	{
		std::cout << "painting Button..." << '\n';
	}
protected:
	virtual void clear_background() override
	{
		Widget::clear_background();
		std::cout << "clearing  Button background..." << '\n';
	}
};

class CheckBox : public Button
{
private:
	virtual void paint() override
	{
		std::cout << "painting CheckBox..." << '\n';
	}
protected:
	virtual void clear_background() override
	{
		Button::clear_background();
		std::cout << "clearing CheckBox background..." << '\n';
	}
};


====================================================================================================================
====================================================================================================================
====================================================================================================================
====================================================================================================================


TEMPLATE METHOD
---------------
NVI, template metodun bir alt kümesi.GOF taki behavioral tasarımlardan biri.
Kalıtımda en çok kullanılan yapı zaten. Pek çok insan tasarım kalıbı olduğunu bilmeden kullanıyor.

1 - Taban sınıfa bir kontrol noktası vermiş oluyoruz.
Taban sınıfın sanal funcı public olsaydı bu durumda
doğrudan clientlar taban sınıfın sanal funcını çağırdığında türemiş sınıfın overrideı çalışacaktı.
Şimdi türemişin sınıfın override çalışmasından önce taban sınıfın sanal olmayan funcı bir takım kontroller yapabiliyor.
Yani türemiş sınıfın overrideının bazı koşulları sağladığında çalışmasını sağlıyor.

2 - Ortak kod bir yere toplanmış oluyor.
Taban sınıfın dtoru virtual olmazsa, taban sınıf poğinterı ile türemiş sınıf nesnesini delete edersek tanımsız davranış
olacak. Dolayısı ile taban sınıf ptr ile türemiş sınıf nesnesi delete edilecekse taban sınıfın destructoru public virtual
olması gerekiyor.
Öyle durumlar var ki, dinamik ömürlü türemiş sınıf nesnesinin taban sınıf ptr ile delete edilmesini istemiyoruz.
bu durumda taban sınıfın dtoru virtual olmasına gerek yok. Bu durumda da taban sınıfın dtorunun clientlar tarafından
doğrudan çağrılması için protected yapıyoruz.

Template method
Burada template diye basedilen bildiğimiz template değil.Algoritmanın nasıl çalışacağını söylüyor.

Bir algoritma var taban sınıf tarafından kullanılacak, algoritmayı saptayan taban sınıf.
Algoritmanın hangi basamaklardan oluştuğu, bu basamakların neler olduğu taban sınıf tarafından
biliniyor.

a - Bu basamakların ne şelilde implemente edileceği konusunda gerekli bilgi yok,
	yada aşağıdaki sınıflar tarafından belirleniyor.Algoritmanın genel iskeletini hazırlıyor ama bir veya daha fazla
	basamağının kodunu implementasyonunu türemiş sınıflara bırakıyor.

26.30 Bir şekil üzerinden anlatılıyor.

-------------------------------------------------------------------------------------------------------------------

pizza.h
-------
#include<string>
class Pizza
{
public:
	Pizza(std::string name);
	~Pizza() = default;
	void prepare();  //NVI

protected:
	void prepare_dough();
	virtual void add_sauce();
	virtual void add_bake();
private:
	virtual void add_toppings() = 0;	// Aynı zamanda bir customization point verdik.
};


pizza.cpp
---------
void Pizza::prepare()	// ALGORITMANIN İSKELETİ HAZIRLANDI
{
	prepare_dough(); // 1.pizzanın hamuru
	add_toppings();  // 2.diğer adım
	bake();			 // 3.Pişirme adımı
	add_sauce();	 // 4.sos ekliyor
}


veggie_pizza.h
--------------
#include "pizza.h"

class Veggie : public Pizza
{
public:
	virtual void add_sauce()override;
	virtual void bake()override;
private:
	virtual void add_toppings()override;   // implementasyonları yapmıyoruz
protected:
};


main.cpp
--------

#include "pizza.h"
#include "veggie_pizza.h"

void func()
{
	Pizza *p = new Veggiepizza{"vegpizza"};

	p->prepare();	// prepare çağrıldığında veggiepizzanın overrideları çağrılacak.
}

Gerekli kodların yazıldığını düşünelim.karmaşık değil.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

BİRKAÇ TUPLE TEKNİĞİ
--------------------

int main()
{
	int x = 23, y = 34;

	std::cout << "x = " << x << " y = " << y << '\n'; // X ile Y nin değerlerini takas etmek istiyoruz.

	//std::tie(x,y); // tuple<int&,int&> return değeri bu türden.Referans tuple döndürdü yani.

	tie(x,y) = pair{y,x};

	std::cout << "x = " << x << " y = " << y << '\n'; //Elemanlar yer değiştirdi.
}

-------------------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std;

	int x = 10, y = 20, z= 30, t = 40;

	tie(x,y,z,t) = tuple{y,z,t,x}; // Az öncekinden bir farkı yok.

	std::cout << "x = " << x << '\n';
	std::cout << "y = " << y << '\n';
	std::cout << "z = " << z << '\n';
	std::cout << "t = " << t << '\n';

}

Not: Tuple da karşılaştırma pairdeki gibi. 1. elemanlar eşitse 2. ye bakılıyor, 2. de eşitse 3. ye bakılıyor ..
diye gidiyor.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

class Date{
public:
	friend bool operator<(const Date&, const Date&)
	{
		return true;
	}
};

class Myclass{
public:
	friend bool operator<(const Myclass &m1, const Myclass &m2)
	{
		std::tuple{m1.mx, m1.mname, m1.msurname, m1.mbdate} < std::tuple{m2.mx, m2.mname, m2.msurname, m2.mbdate};
	}
private:
	int mx{};
	std::string mname;
	std::string msurname;
	Date mbdate;

};

-------------------------------------------------------------------------------------------------------------------

C++ 17 de tuple ile ilgili bir problemi çözdüler.

std::apply

int sum(int x, int y, int z)
{
	return x + y + z;
}

int main()
{
	std::tuple t{1,6,9};

	std::cout << sum(get<0>(t), get<1>(t), get<2>(t)) << '\n'; // 16

	std::cout << std::apply(sum,t) << '\n'; // 1. parametre callable, 2 parametre tuple. 16 verdi.

}

===================================================================================================================
===================================================================================================================
===================================================================================================================

STRATEJİ TASARIM KALIBI
-----------------------
Policy based design'a geçiş aracı olacak.
Strateji ile template metod birbirine baya benziyor. kritik farklılık aslında birinin alkgoritmanın steplerinin
implementasyonunu türemiş sınıflara bırakması, ama stratejide algoritmanın kendisini customize etme imkanı veriyor.

Policy design pattern, strateji paternini runtimedan compile time a alınması ile ilgili.


context.h
---------
#pragma once
#include <memory>

class Strategy;

class Context{
public:
	Context(Strategy*);
	void do_something();
	void set_strategy(Strategy*);
private:
	std::unique_ptr<Strategy> mps;
};


context.cpp
-----------
#include "context.h"
#include "strategy.h"

Context::Context(Strategy*p) : mps{p}
{

}

void Context::do_something()
{
	std::cout << "context is doing something as per its buseiness logic\n";
	mps->do_algorithm();
}

void Context::set_strategy(Strategy *p)
{
	mps.reset(p);
}


strategy.h
----------
#pragma once

class Strategy{
public:
	virtual ~Strategy() = default;
	virtual void do_algorithm() = 0;
};


necstrategy.h
-------------
#pragma once

#include "strategy.h"

class NecStrategy : public Strategy{
public:
	NecStrategy();
	~NecStrategy();
	virtual void do_algorithm()override;
};

necstrategy.cpp
---------------
#include "necstrategy.h"
#include <iostream>

NecStrategy::NecStrategy()
{
	std::cout << "Neco Strategy object created\n";
}

NecStrategy::~NecStrategy()
{
	std::cout << "Neco Strategy object destroyed\n";
}

void NecStrategy::do_algorithm()
{
	std::cout << "Neco Strategy is been used in implementation!\n";
}


ergstrategy.h
-------------
#pragma once

#include "strategy.h"

class ErgStrategy : public Strategy{
public:
	ErgStrategy();
	~ErgStrategy();
	virtual void do_algorithm()override;
};


ergstrategy.cpp
---------------
#include "ergstrategy.h"
#include <iostream>

ErgStrategy::ErgStrategy()
{
	std::cout << "Ergin Strategy object created\n";
}

ErgStrategy::~ErgStrategy()
{
	std::cout << "Ergin Strategy object destroyed\n";
}

void ErgStrategy::do_algorithm()
{
	std::cout << "Ergin Strategy is been used in implementation!\n";
}


main.cpp
--------

#include "necstrategy.h"
#include "ergstrategy.h"
#include "context.h"


int main()
{
	Context c{new NecStrategy};
	c.do_something();
	std::cout << '\n';

	//------------------------------------------

	c.set_strategy(new ErgStrategy);
	c.do_something();

}


===================================================================================================================
===================================================================================================================
===================================================================================================================
===================================================================================================================
===================================================================================================================


POLICY BASED DESIGNED
--------------------
En çok kullanılan C++ patern/idiyomlarından biri.Strateji kalıbının derleme zamanı / static versiyonu.
Policy tabanlı tasarım templatelerle gerçekleştiriliyor static olarak. Kodda policy yi bir template in tür
parametresi yapılıyor. Aslında ayrı ayrı kodlar sınıflar oluşturmak yerine aynı template ten farklı tür argümanları kullanarak
farklı davranış özelliklerine sahip sınıflar oluşturuyruz. Aslında STL kullanırken heryerde kullanıyoruz bunu.

Container sınıfların allocator parametresi tipik örneği. Dİnamiz dizi sınıfı, vector...
O template parametresi olmasaydı kendi kodunda bellek alanı etme stratejisini kendisi seçseydi bu herkese
uygun olmayacaktı. O zaman başka bir allocation stratejisi kullanmak istediğimizde başka bir sınıfa daha
ihtiyacımız olacaktı. Böylece davranışsak farklılık belirli bir noktada olmasına rağmen komple yeni bir
sınıf kullanılmış olacaktı. Şimdi de yeni bir sınıf kullanıyoruz ama tmeplate olduğundan compile timeda
derleyici kodunu yazıyor.Farklı bir allocator kullanmak istersek kendi türümüzü kullanıyoruz.

Bu strateji için template argümanı talep eden sınıfa, farklı farklı davranışa sahip birsürü sınıf oluşmasını sağlayacak
sınıfa host class deniyor(mesela vector host class). Davranışı özelleştirmek için template argümanı olarak verilen
sınıfa policy class deniyor.

Amaç
Policy tabanlı tasarım yapılınca esneklik sağlanmış oluyor.Ortada tek bir sınıf kodu var ama o sınıf çok daha esnek.
İstediğim gibi değiştirebilirim. Farklı bir davranış biçimi istersem farklı bir template argümanı kullanıyorum.
Aynı sınıf şablonundan bu sefer o template argümanıyla stratejiyi özelleştirecek yeni bir sınıf oluşturuyorum.
Genişletilebilirlik, böylece farklı farklı davranışları implemente eden policy sınıfları oluşturarak yepyeni sınıflar elde
edebiliyorum. Strateji birden fazla noktada olabilir.

Avantajları olduğu gibi dezavantajlarıda var.
Dezavantajlarından biri, bunların ayrı sınıflar olması. herhangibir araç özellikle oluşturulmazsa birbirine atanabilir değil.
Mesela farklı allocator türüne ait vectorler birbirine atanabilir olmayacaklar.
Mesela uniqueptr nin farklı deleter sınıfları birbirine atanabilir değil.

Policy based designed idiyomunun merkezinde 1 veya daha fazla tür parametresine sahip
sınıf şablonu var. Böyle sınıflara host class deniyor.

template<typename T, typename AllocationPolicy, typename DeletePolicy> // Bu hostclass
class Container{

};

AllocationPolicy burada allocation prensibi diye geçiyor.
Fiilen belirli bir interfacei sağlayan template argümanı olarak kullanılan sınıfada policy class deniyor.

Bizim host classımız olacak client kodlar hostclass şablonunu kendi seçtikleri template 
argümanı ile instantiate edecek, oluşan her specialization ayrı davranışsal özellikler
sağlayan yeni bir sınıf olarak kullanılacak.

NOT !!
Herhangibir template için bir instantiation sonucu derleyici tarafından oluşturulan kod o şablonun
bir specializationı.

specialization --> Açılım

vector<int> --> vectörün int specializationu.Derleyici vector sınıf şablonundan 1. template
		parametresi için int tür argümanı kullanılınca bir sınıf oluşturuluyor ve buna specialization deniyor.

Explicit Specialization --> Derleyici belli bir tür için primary template ten instantiation yoluyla
			    mesela int specializationu oluşturma, ben int specializationının kodunu veriyorum demek.
 			    Primary templateten oluşturmak yerine benim verdiğimi kullan demek

Partial Specialization --> Belirli bir tür kümesinin specializationu için alternatif veriyoruz.

Specialization -> Açılım

Bir interface e uygun olarak oluşturmuş sınıflara policy sınıfları deniyor.
İnterface in kendisinede policy deniyor.

Mesela her policy sınıfının create funcı olacak. Hostlar buna güvenecek mesela.
Her policy sınıfın value_type isimli type memberı olabilir.

------------------------------------------------------------------------------------------------------------------

ÖR: HELLO WORLD POLICY BASED DESIGN

Policy based design da temel iki araç var.

- Bir tanesi kalıtım. Farklı farklı policy leri biz kalıtım yoluyla kullanabiliriz.
Amaç interface i kendi interfaceine katmak değil, yani is a değilde has a ilişkisi için
kullanacağımız için public yerine private kalıtım kullanacağız çoğunlukla. Bazı durumlarda
ise taban sınıf olarak kullanılan sınıflar türemiş sınıfın public interface ine belirli func veya
türleri enjekte etmek isterse public kalıtım kullanacağız.

- Diğeri ise kalıtım yerine oluşturulacak sınıfın PolicyClasslar türünden elemanlara sahip olması
yani direk composition yapmak.

2 yöntemde composition. Biri containment yoluyla composition diğeri ise kalıtım ile composition.
C++ 20 ye kadar composition yerine kalıtım kullanılıyordu. Nedeni policy classların yani 
template argümanı olarak kulanılacak sınıfların çoğu zaman empty class olması ve kalıtım
kullandığımızda bu durumda empty classtan private kalıtımı yapınca Empty base optimization
yapılıyor ama C++20 ile [[no_unique_address]] attribute	ile artık empty classlar türünden
elemanlar sözkonusu olduğunda derleyicinin bellek alanı kullanımı açısından bir optimizasyon yapıyor.

//		print			get_message
template <typename OutputPolicy, typename LanguagePolicy>
class HelloWorld : private OutputPolicy, private LanguagePolicy
{
public:
	//behavior method
	void run()const
	{
		print(get_message()); // Böyle yazamayız.Taban sınıfta bile olsa aramaz	
				      // sınıf şablonu olmasaydı bu ismi önce local alanda arayacaktı
				      // bulamazsa helloworldte arayacaktı orada da bulamazsa taban sınıfta
				      // aranacaktı. Template olunca aramaz.
		
		// TABAN SINIFLARDA ARANMASI İÇİN NELER YAPILABİLİR.
		// 1 - Using bildirimi. Private bölümde de yapılabilir.
			   using LanguagePolicy::get_message;
			   using LanguagePolicy::print;
		
		// 2 - Qualified name kullanarak.This ile
			   this -> print(this->get_message());
		
		// 3 - Qualified name kullanarak.Sınıf ismi ile 
			   OutputPolicy::print(LanguagePolicy::get_message());
	}
private:
	using LanguagePolicy::get_message;
	using LanguagePolicy::print;
};


//BU POLICY CLASS.YUKARIDA ARGUMAN OLARAK KULLANILABİLİR
class CoutOutputPolicy{
protected:

};

--------------------------------------------------------------

//POLICY CLASS.TEMPLATE OLABİLİR
template<typename T>
class CoutOutputPolicy{
protected:
	void print(T &&)
	{
	}
};

--------------------------------------------------------------

//POLICY CLASS. BURADADA FUNC TA EMPLATE OLABİLİR
class CoutOutputPolicy{
protected:
	template<typename T>
	void print(T &&x)
	{
		std::cout << x << '\n';
	}
};

------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>

template <typename OutputPolicy, typename LanguagePolicy>
class HelloWorld : private OutputPolicy, private LanguagePolicy
{
public:
	//behavior method
	void run()const
	{
		OutPutPolicy::print(languagePolicy::get_message());
	}
private:
};


class CoutOutputPolicy{
protected:
	template<typename T>
	void print(T &&x)const
	{
		std::cout << x << '\n';
	}
};

class EnglishLanguagePolicy
{
public:
	std::string get_message()const
	{
		return "Hello World!";
	}
};


class TurkishLanguagePolicy
{
public:
	std::string get_message()const
	{
		return "Merhaba Dunya!";
	}
};


class GermanLanguagePolicy
{
public:
	std::string get_message()const
	{
		return "Halo Welt!";
	}
};


using English = HelloWorld<CoutOutputPolicy, EnglishLanguagePolicy>;
using Turkish = HelloWorld<CoutOutputPolicy, TurkishLanguagePolicy>;
using German = HelloWorld<CoutOutputPolicy, GermanLanguagePolicy>;


int main()
{
	English e;
	German g;
	Turkish t;

	e.run();
	g.run();
	t.run();
}

*/
