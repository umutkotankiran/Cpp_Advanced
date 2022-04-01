/*

DECORATOR PATERNINDEN DEVAM
---------------------------
Bir nesnenin davranışını değiştirmek için akla ilk gelen yok kalıtım mekanizması.
Ancak kalıtımın bazı istenmeyen özellikleri sözkonusu olabilir.

Öyle durumlar varki kalıtımın bizim için istenmeyen özelliklerivar yani
işimize gelmiyor.

Kalıtım Static yani compile time a ilişkin bir yapı. Hayatta olan bir nesnenin
davranışını programın çalışma zamanında değiştiremeyiz. Bunun için o nesnenin
yerine farklı bir alt sınıf türünden (derived class) nesne kullanmalıyız.

Runtime polimorfizmi var tabi. Buna daha iyi seçenek kalıtım yerine composition
kullanmak. Composition terimi nesne yönelimli programlamadaki composition ile
C++ taki containment birbirine karıştırılmamalı.Aslen o da bir composition.
Farklı sınıf türünden elemanları member olarak alıp bunların kullanıldığı
arayüz oluşturabiliriz.Diğer dillerde composition denilince, composition
yine polymorphic bir nesnenin içerilmesi.Bununda C++ karşılığı
taban sınıf türünden pointer yada referans.

Compositionu kalıtıma tercih et denildiğinde containment kastedilmiyor.
Composition yoluyla oluşacak polymorhismi, doğrudan kalıtım yoluyla oluşacak
polimorhisme tercih et anlamında.
Polimorfik bir yapıdan bahsedilmiyorsa hoca containment diyor buna.Kendi tercihi.

Bir çok programlama dilinde kalıtımda bir sınıfın tek bir taban sınıfı olabilir.
Birçok dilde C++ dilinde olan çoklu kalıtım aracı yoktur.

NOT : Virtual inheritance, multiple inheritance mülakatlarda çok soruluyor.

Multiple inheritance sağlamanın yollarından biri kalıtım yerine composition kullanmak
olabilir. Birleştirmede bir nesne başka bir nesneye referans tutulabilir ve referans
yoluyla bağladığı nesneye bazı işleri delege edebilir. Kalıtımda ise nesne taban sınıftan
aldığı davranışı gerçekleştirir.

Birleştirmede bağlantıda bulunduğumuz nesneyi programın çalışma zamanında başka bir
nesne ile değiştirebiliriz. Böylece sahip olan nesnenin davranışı programın çalışma
zamanında değişebilir.
Böylece bir nesne programın çalışma zamanında farklı farklı sınıfların davranışlarını
kullanabilir. Yada nesne birden fazla nesneye referans tutabilir. Farklı işleri
farklı türden nesnelere delege edebilir.


DECORATOR
---------
Bir nesneye dinamik olarak ilave sorumluluklar(davranışlar) yükler.
İşlevselliğin arttırılması için kalıtıma karşı esnek bir seçenek yükler.
Static değil dinamik bir seçenek oluşturur.

Decoratorun diğer ismide wrapper(sarmalayıcı). Burada ilave davranış kazandırmak istediğimiz
nesneyi sarmalıyoruz.

Wrapper hedef nesneyle aynı metolara sahiptir. Aldığı istekleri sarmaladığı hedef nesneye
delege eder. Ancak sarmalayıcı nesne, istediği sarmaladığı hedef nesneue geçmeden önce
yada geçtikten sonra bir iş yaparak hedef nesnenin davranışını değiştirebilir.

Basit bir sarmalayıcı nesne ne zaman / nasıl gerçek bir decorator olur?
Sarmalayıcı nesne sarmaladığı nesne ile aynı arayüze sahiptir, aynı arayüzü implemente eder.

Clientın bakış açısından dekorator nesnesi ile hedef nesne arasında bir fark yok.
Wrapper nesne aynı arayüzü sağlayan herhangibir nesneye referans tutabilir.
Böylece bir nesneyi sırayla birden fazla decorator ile sarmalayabiliriz.

-------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------

ÖR:

class Component{
public:
	virtual void do_it() = 0;
	virtual ~Component() = default;
};


class ConcreteComponent : public Component
{
public:
	~ConcreteComponent()
	{
		std::cout << "~Concrete Component\n";
	}

	void do_it()override
	{
		std::cout << "ConcreteComponent::doit()\n";
	}
};


class Decorator : public Component
{
public:
	Decorator(Component *pcom) : mp_wrappee{pcom} { }
	
	~Decorator()
	{
		delete mp_wrappee;
	}

	void do_it()override
	{
		mp_wrappee -> do_it();
	}

private:
	Component *mp_wrappee;
};


class XDecorator : public Decorator
{
public:
	XDecorator(Component *pcor) : Decorator{pcor} { }  //DİKKAT !!!!!  Bunun yerinde Inherited Ctor da kullanılabilirdi
	~XDecorator()
	{
		std::cout << "~XDecorator	";
	}

	void do_it()override
	{
		std::cout << "XDecorator added behavior\n";
		Decorator::do_it();
	}
};


class YDecorator : public Decorator
{
public:
	YDecorator(Component *pcor) : Decorator{pcor} { } //DİKKAT !!!!!	Bunun yerinde Inherited Ctor da kullanılabilirdi
	~YDecorator()
	{
		std::cout << "~YDecorator	";
	}

	void do_it()override
	{
		std::cout << "YDecorator added behavior\n";
		Decorator::do_it();
	}
};



class ZDecorator : public Decorator
{
public:
	ZDecorator(Component *pcor) : Decorator{pcor} { }  //DİKKAT !!!!!	Bunun yerinde Inherited Ctor da kullanılabilirdi
	~ZDecorator()
	{
		std::cout << "~ZDecorator	";
	}

	void do_it()override
	{
		std::cout << "ZDecorator added behavior\n";
		Decorator::do_it();
	}
};


int main()
{
	Component *pyx = new YDecorator{new XDecorator{new ConcreteComponent}};	
	pyx -> do_it();

	delete pyx;

	//-------------------------------------------------------------------

	Component *pzxy = new ZDecorator{new XDecorator{new YDecorator{new ConcreteComponent}}};
	pyx -> do_it();

	delete pyx;	
	
	//-------------------------------------------------------------------

	//tekrar Y ile sarmaladık.
	Component *pyzxy = new YDecorator{new ZDecorator{new XDecorator{new YDecorator{new ConcreteComponent}}}};
	pyx -> do_it();

	delete pyx;

}

MOLA

==================================================================================================================================
==================================================================================================================================
==================================================================================================================================
==================================================================================================================================
==================================================================================================================================


PROXY PATTERN
-------------
Proxy yapısal tasarım kalıplarından biridir.
Bir başka varlığın yerine geçen anlamında kullanılıyor. Vekil.

En çok kullanılan paternlerden biri.
Proxy nesne orijinal nesneye erişimi kontrol eder.İstemciden gelen
istediği orijinal nesneye iletmeden önce ya da ilettikten sonra birtakım
işlevlerin yürütülmesini sağlar.

Bir nesneye erişimi neden kontrol altına almak istiyoruz ?
1 - Nesnenin kendisine doğrudan erişim güvenlik riski içeriyor olabilir.
2 - İhtiyaç duyulacağı ana kadar nesne oluşturmayı geciktirebilir ve
böylece milayet azaltılabilir.(belkide nesneye ihtiyaç olmayacak)
3 - Nesne uzakta olabilir.Ona erişim dışarıdan yalıtılmıştır. Ona kontrollü
ve kolaylaştırılmış ve detaylardan arındırılmış bir erişim sağlanabilir
4 - Nesneye eriim thread safe olmayabilir.Erişim thread safe hale getirilebilir.
5 - Nesneye erişimden önce yada sonra farklı işler yapılıyor olabilir.
ÖR: Erişim işlemleri loglanabilir. Nesneye erişimde bir kuyruk sistemi oluşturulabilir.
Nesneye erişim senkron olmaktan çıkıp asenkron hale getirilebilir.

-----------------------------------------------------------------------------------------------

ÇÖZÜM:
Client ile iletişimi kontrol edilecek servis nesnesi arasına bir geçiş nesnesi
yada servis nesnesi gibi davranacak bir vekil nesne koyulur. Bu proxy nesnedir.

Proxy nesnesinin arayüzü saklanan nesne ile aynı olursa, istemci bu durumdan
haberdar olmaz. İstemci orijinal nesne ile aradaki nesneyi ayırt edemez.

Proxy nesne istemciyi asıl nesneden yalıtır ve asıl nesneye olan bağımlılığı ortadan kaldırır.

Proxy nesne kısıtları yöneterek asıl nesnenin birlikteliğini(cohesion) yönetmeye yardımcı olur.

-----------------------------------------------------------------------------------------------

PROXY KALIBINDAN FARKLI ŞEKILLERE FAYDALANABILIRIZ:

Bazı hizmet nesneleri hayatta oldukları sürece (yüksek miktarda) sistem kaynaklarını kullanır. 
Oysa bunlara her zaman ihtiyacımız olmayabilir. Uygulama başlar başlamaz böyle hizmet nesnelerini 
hayata başlatmak yerine bunlara ilk kez ihtiyaç duyulduğunda bu nesneleri hayata getirebiliriz. (lazy initialization / lazy loading).
Bu amaçla kullanılan Proxy nesnelerine "Virtual Proxy" denir.

Bir servis nesnesi işletim sisteminin kritik parçalarına ilişkin olabilir. 
Her kodun bu nesneye erişebilmesi bir güvenlik açığı oluşturabilir. 
Yalnızca belirli istemcilerin servis nesnesini kullanabilmesi gerekebilir. 
Proxy nesne yalnızca belirli kriterleri sağlayan istemcilerin isteklerini asıl nesneye iletebilir. 
Bu amaçla oluşturulan Proxy nesnelerine "Protection Proxy" denir.

Bir hizmet nesnesi uzaktaki (farklı bir adres uzayındaki) sunucuda tutuluyor olabilir.
Bu durumda Proxy nesnesi isteği service nesnesine network üzerinden gönderir. 
Proxy nesnesi network ile ilgili tüm detayları gizler. 
Böyle Proxy nesnelerine "Remote Proxy" denir.

Bazı durumlarda Proxy sınıfları caching amaçı kullanılır. 
Proxy sınıfı tekrar eden istekler için bir caching mekanizması oluşturur. (Caching Proxy)

ÖR:

subject.h
---------
#pragma once

class Subject{
public:
	virtual void request() = 0;
	virtual ~Subject() = default;
};

real_subject.h
---------------
#pragma once

#include "subject.h"

class RealSubject : public Subject
{
public:
	void request()override;
};


proxy.h
--------
#pragma once
#include "subject.h"

class RealSubject;

class Proxy : public Subject
{
public:
	Proxy();
	~Proxy();
	void request()override;
private:
	RealSubject* mp_subject;
};


real_subject.cpp
----------------
#include "real_subject.h"
#include <iostream>

void RealSubject::request()
{
	std::cout << "[RealSubject] request processed\n";
}

proxy.cpp
---------
#include "proxy.h"
#include "real_subject.h"
#include <iostream>

Proxy::Proxy()
{

}

Proxy::~Proxy()
{
	delete mp_subject;
}

void Proxy::request()	//Bu virtual proxy örneği
{
	if(!mp_subject)
	{
		std::cout << "[Proxy] Creating RealSubject\n";
		mp_subject = new RealSubject{};
	}

	std::cout << "[Proxy] Additional behavior\n\n";
	mp_subject->request();
}


main.cpp
--------

#include "proxy.h"
#include "real_proxy.h"
#include "subject.h"

void foo(Subject *p)
{
	ps->request();
	ps->request();
	ps->request();

	//
}

int main()
{
	Subject *ps = new Proxy{};
	foo(ps);
	delete ps;
}

-----------------------------------------------------------------------------------------------

ÖR:

PROXY_2

image.h
-------
#include <string>

class Image
{
public:
	Image() = default;
	Image(const std::string& file_name);

	std::string get_file_name()const;
	virtual ~Image() = default;
	virtual void display()const = 0;
	virtual void load() = 0;
	virtual void load(const std::string& file_name) = 0;
protected:
	void set_fine_name(const std::string& file_name);
private:
	std::string m_file_name; 

};

image.cpp
---------
#include "Image.h"

void Image::set_file_name(const std::string& file_name)
{
	m_file_name() = file_name;
}

Image::Image(const std::String& file_name) : m_file_name{file_name} {}

std::string Image::get_file_name()const
{
	return m_file_name;
}


bitmap.cpp
----------

void Bitmap::load_impl()
{
	m_buffer.clear();
	std::ifstream file{get_file_name()};
	if(!file)
	{
		throw std::runime_error{"Failed to open file"};
	}

	std::string line{};
	std::cout << "Loading bitmap[";

	while(std::getline(file,line))
	{
		using namespace std::chrono_literals;
		m_buffer += line + '\n';
		std::this_thread::sleep_for(100ms);
		std::cout << '.';
	}
	std::cout << "] Done!\n";
}

Bitmap::Bitmap(const std::string& file_name) : Image{file_name} { }

void Bitmap::display()const
{
	std::cout << m_buffer;
}

void Bitmap::load()
{
	load_impl(); 
}

void Bitmap::load(const std::string& file_name)
{
	set_file_name(file_name);
	load();
}



bitmap_proxy.h
--------------
#pragma once
#include "image.h"

class Bitmap;

class BitmapProxy : public Image
{
public:
	BitmapProxy();
	BitmapProxy(const std::string& file_name);
	~BitmapProxy();
	void display()const override;
	void load()override;
	void load(const std::string& file_name) override;

private:
	Bitmap* m_pbitmap{};
	std::string m_file_name{};
	bool m_loaded{false};
}

bitmap_proxy.cpp
----------------

#include "bitmap_proxy.h"
#include "bitmap.h"
#include <iostream>

BitmapProx::BitmapProxy() : BitmapProxy{""} {}

BitmapProxy::BitmapProxy(const std::string& file_name)
{
	m_pbitmap = new Bitmap{file_name};
}

void BitmapProxy::display()const
{
	if(!m_loaded)
	{
		std::cout << "bitmap proxy is loading bitmap image\n";
		if(m_file_name.empty())
		{
			m_pbitmap->load();
		}
		else
		{
			m_pbitmap->load(m_file_name);	
		}
	}
	m_pbitmap->display();
}

void BitmapProxy::load()
{
	 m_file_name.clear();
}

void BitmapProxy::load(const std::string& file_name)
{
	m_file_name = file_name; 
}

???????????????????????????????????????????
bitmap.h yok
???????????????????????????????????????????


main.cpp
--------

#include "bitmap.h"
#include "bitmap_proxy.h"

int main()
{
	Image *p_img = new BitmapProxy{"banner.txt"};
	p_img->load();
	//p_img->display(); 
	p_img->load("Smiley.txt");
	p_img->display();
	delete p_img;

	//--------------------------------------------------

	Image *p_img = new BitmapProxy{"banner.txt"};
	p_img->load();
	(void)getchar();
	p_img->display();
	(void)getchar();
	// p_img->load("Smiley.txt");
	// p_img->display();
	delete p_img;
}

Bu kod çok iyi bir örnek değil.Daha iyisini hoca yükleyecek.


=============================================================================================
=============================================================================================
=============================================================================================
=============================================================================================

FACADE
------
Alt sistemler geliştirildil.e daha karmaşık hale geliyorlar.
Bir alt sistemde çok sayıda bileşen var ve bunlar ayrı arayüzlere sahipler.
Bu karmaşık arayüzler nedeniyle istemcilerin altsistemin işlevselliğinden faydalanması zorlaşıyor.

Karmaşık func sistemini nasıl daha kolay hale getiririz ? 
Farklı programlama paradigmalarını nasıl daha kolay ve daha sezgisel olarak kullandırabiliriz?
Nesne yönelimli olmayan API leri verileri fonksiyonları nesne yönelimli sağlam taşınabilir yapışkanlığı
yüksek bir yapı haline nasıl getirebiliriz.

İstemciler karmaşık alt sistem ile yüksek bir coupling istemiyorlar.
İstemicilerin alt sistemle doğrudan etkileşime girmesi yerine araya bir katman ekliyoruz.Bu katman
müşterilere alt sistemdeki bileşenlerin işlevselliğini daha basit bir arayüzle sunuyor.
Oluşturulan bu katmana "facade"(cephe) deniyor.

Çözüm : İstemciler tarafından kullanılacak daha basit ve daha yüksek seviyeli bir arayüz oluşturur.
		İstemci kodlar büyük alt sistemin getirdiği karmaşıklıktan arındırılır.

Neler Sağlanıyor ? 
------------------
İstemcileri karmaşık arayüze sahip bileşenlerden izole ediyor.
İstemcinin etkileşimde bulunduğu nesne sayısı azalır. Böylecce coupling azalır.
Hizmet veren bileşenler değiştirğinde istemci kodun değişmesi gerekmiyor.
Büyük sistemlerde compile time ilişkin bağımlılıklar azalır.
Alt sistemin kullandığı sınıflar gizlenmez. İstemci kodlar halen bu sınıfları kullanabilirler.
Alt seviyedeki işletim sitemi fonksiyonları Apileri veri yapıları için nesne yönelimmi bir arayüz sağlar.
Bir facade bir func veya sınıfların oluşturduğu bir küme olabilir.
Bir sistem için birden fazla facade oluşturulabilir.Oluşturulan her facade belirli bir işlevsellik 
için arayüz oluşturabilirler.
Böylece istemci ile alt sistemin sınıfları arasındaki coupling azaltılabilir.
Yeni soyutlama  eklendiği için performans düşüşüde olabilir
...

Hangi durumlarda Kullanılabilir ? 
---------------------------------
Karmaşık bir sistem için basit bir arayüz oluşturmak istediğimizde istemcilerin çoğunlukla varsayılan olarak
bu basit arayüzü kullanabilirler. Özelleştirme ihtiyacında olan dieğr istemciler alt sistemlerdeki 
sınıfları doğrudan kullanabilirler.

Bir sistem zaman içinde gelişerek daha karmaşık hale gelmiş olabilir.Eski kullanıcılar bu sistemi
daha önceki gibi görebilirler, kullanabilirler. yeni kullanıcılar kendileri için daha basit daha anlaşılır facade
arayüzünü kullanabilirler.

Uygulamamız aşağı seviyeli işletim sistemi apilerine bağlı olabilir. Belirlir bir işletim sistemi ile
bağışıklıktan kaçınmak isteyebiliriz. Bunlar için nesne yönelimli sarmalama yapabiliriz.

Farklı deneyim sebiyesindeki ekip üyeleri aynı sistemi kullanabilirler.
Acemiler ile deneyimliler farklı arayüzleri kullanabilirler.

İstemci kodlarla alt sistemi gerçekleştiren sınıflar arasındaki bağımlılıklar ortadan kaldırılabilir.

*/