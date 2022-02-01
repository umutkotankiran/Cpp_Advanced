/*

Geçen ders sonunda 2 soru vardı onu açıklıyor hoca.

//lazy_init.h
template<typename Tag,typename T>  // 1. Template tür parametresi neden var? Birden fazla static instance yaratabilmek için
class lazy_init{
public:
	constexpr lazy_init() = default; // Lazy init sınıfı türünden global bir nesne default construct edildiğinde constant init edilecek
	T& get()
	{
		static T global;  // T türünden yerel bir nesne
		return global;  // bu nesne döndürülmüş
	}
	T& operator*()
	{
		return get();
	}
	T* operator->()
	{
		return &get();
	}
};

class Myclass{
public:
	Myclass()
	{
		std::cout << "Myclass Constructor this : " << this << '\n';
	}

	~Myclass()
	{
		std::cout << "Myclass Destructor this : " << this << '\n';
	}

	void func()
	{
		std::cout << "Myclass::func this : " << this << '\n';
	}
};

//Tagi olmadan
lazy_init<Myclass>gm1;
lazy_init<Myclass>gm2;


//Tagi ekledik 
lazy_init<struct gm1_tag, Myclass> gm1;
lazy_init<struct gm2_tag, Myclass> gm2;

int main()
{
	//tagi sildikten sonra
	gm1->func();
	gm2->func(); // Tagi silince aynı adresi verecek.Ortada bir tane myclass nesnesi var
                 // Ctor ve dtor bit kere çağrıldı.
	
	//tagi ekledikten sonra
	gm1->func();
	gm2->func(); // Tagi silince farklı adresi verecek.Artık farklı myclass nesneleri var
				 
}

---------------------------------------------------------------------------------------------------------

NOT !!!!!!!!!!!!

 Myclass& foo();

 auto &m = foo();

 NASIL DELETE EDERİM ?
 delete &m; // buradaki & adresini alıyor.

---------------------------------------------------------------------------------------------------------

Usingletondaki deleter yazma olayını açıkladı hoca.
Nested typelar sınıfın private bölümüne erişebiliyorlar.Bu sayede sınıf içerisine
nested type olarak deleter yazıldı.

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

MONOSTATE

#include<string>

class Clock{
public:
	int get_hour()const;
	int get_min()const;
	int get_sec()const;
	std::string get_current_time()const;
private:
	static inline int m_hour{};
	static inline int m_min{};
	static inline int m_sec{};
};

Singletonda tek nesne var.Monostate te isteğe göre birden fazla nesne oluşumuna izin verebiliriz.

Singletonda lazy init yapılabilir. Monostate te lazy init yapılamaz

Singletonda static instance func gerekli. Monostate te bütün public funclar static

Singletonda inheritance ve polymorphism yapılabilir. Static funclar sanal olamaz.

Singletonda var olan bir sınıfı kolayca singleton yapabilirim. Monostate te kolayca yapılamaz kolay değil.

Monostate sınıfların bir çoğu başka bir sınıfı set amaçlı kullanlıyor.

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

BIR SINGLETON IÇIN BIR BASE OLUŞTURACAĞIZ
Her singleton oluşturulduğunda kayıt edilecek ve bunlardan herhangibiri oluşturulabilir.
Bir Singleton için bir base sınıf oluşturacağız.
Bunlar farklı farklı printerlar ama singleton olacaklar ve bir registiry mekanizması olacak.
Yani heer singleton oluştuğunda bir şekilde kayıt edilecek ve biz kayıt edilmiş
singletonlardan herbirini kullanabilecekler.


Kalıtımdan (inheritance) faydalanarak eski kodların yeni kodları kullanılabilmesi sağlanıyor. Coupling azaltılıyor.
Aynı zamanda "lazy evaluation" gerçekleştiriliyor.
Bu da OCP ilkesinin korunması demek
Yukarı seviyedeki kodlar aşağıdaki kodlara bağımlı olmuyor.  Bu da DIP ilkesinin korunması demek
Buradaki kod hangi concrete printer'ın kullanılacağına bağımlı değil
PrinterBase sınıfından yeni bir sınıf kalıtım yoluyla elde edilebilir ve bu fonksiyona gönderilebilir
Ama aynı zamanda printer nesneleri ihtiyaç duyulduğunda oluşturuluyor.

---------------------------------------------------------------------------------------------------------

//printerbase.h
---------------
#include<string>
#include<memory>

class PrinterBase{
protected:
	PrinterBase() = default;

public:
	virtual ~PrinterBase() = default;
	PrinterBase(const PrinterBase&) = delete;
	PrinterBase& operator=(const PrinterBase&) = delete;
	virtual void print(const std::string&) = 0;
};

using PrinterPtr = std::shared_ptr<PrinterBase>;


//html_printer.h
----------------
#include "printerbase.h"

class HtmlPrinter : public PrinterBase{
	HtmlPrinter();
public:
	~HtmlPrinter();
	static PrinterPtr get_instance();
	virtual void print(const std::string)override;
};


//PdfPrinter.h
----------------
#include "printerbase.h"

class PdfPrinter : public PrinterBase{
	PdfPrinter();
public:
	~PdfPrinter ();
	static PrinterPtr get_instance();
	virtual void pirnt(const std::string)override;
};


//xml_printer.h
---------------
#include "printerbase.h"

class XmlPrinter : public PrinterBase{
	XmlPrinter();
public:
	~XmlPrinter ();
	static PrinterPtr get_instance();
	virtual void pirnt(const std::string)override;
};

//html_printer.cpp
------------------
#include<iostream>
#include "html_printer.h"

HtmlPrinter::HtmlPrinter()
{
	std::cout << "Html printer olusturuldu\n";
}

HtmlPrinter::~HtmlPrinter()
{
	std::cout << "Html printer yok edildi\n";
}

PrinterPtr HtmlPrinter::get_instance()
{
	static PrinterPtr ptr{ new HtmlPrinter{}};
    return ptr;
}

void HtmlPrinter::print(const std::string &str)
{
	std::cout << "Html Printer : " << str << '\n'; 
}


//PdfPrinter.cpp
----------------
#include<iostream>
#include "pdfPrinter.h"

PdfPrinter::PdfPrinter()
{
	std::cout << "PDF printer olusturuldu\n";
}

PrinterPtr PdfPrinter::get_instance()
{
    return PrinterPtr{ new PdfPrinter{} };
}

PrinterPtr PdfPrinter::print(const std::string& str)
{
    return PrinterPtr{ new PdfPrinter{} };
}

PdfPrinter::~PfdPrinter()
{
	std::cout << "PDF printer yok edildi\n";
}



//xml_printer.cpp
-----------------
#include "xml_printer.h"

XmlPrinter::XmlPrinter()
{
    std::cout << "XML Printer olusturuldu\n";
}

XmlPrinter::~XmlPrinter()
{
    std::cout << "XML Printer yok edildi\n";
}

PrinterPtr XmlPrinter::get_instance()
{
    static PrinterPtr ptr{ new XmlPrinter{}};
	return ptr;
}

void XmlPrinter::print(const std::string &str)
{
	std::cout << "Xml Printer : " << str << '\n';
}

SINGLETON PRNTERLARI PRİNTER PROVİDER BİR ŞEKİLDE MAPTA TUTACAK
BÖYLECE PRİNTERLARI İSTEYEN ASLINDA ASLINDA BİR STRİNG İLE PRİNTERI İSTEYECEK.

//printer_provider.h
--------------------
#include<functional>
#include<unordered_map>
#include "printerbase.h"

using CreatorFunc = std::function<PrinterPtr()>; // PrinterPtr() bu bir func türü. 
// using PrinterPtr = std::shared_ptr<PrinterBase>;  // header filedan include edildi zaten

class PrinterProvider{
	struct InstanceData{
		PointerPtr m_printer;
		CreatorFunc m_creator;
	};
	inline static std::unordered_map<std::string,InstanceData> mprinter_map{};

public:
	static void register_creator(const std::string &key, CreatorFunc func);
	static PrinterPtr get_printer(const std::string& key);
};


//printer_provider.cpp
----------------------
#include "printer_provider.h"
#include <iostream>

void PrinterProvider::register_creator(const std::string &key, CreatorFunc func)
{
	if(!mprinter_map.contains(key))
	{
	    mprinter_map[key].m_creator = func;  // [] key yoksa ekliyor varsada değerini get ediyor.
	    std::cout << key << " singletonu kayit edildi\n";
    	}
	else    //varsa buraya girer
	{
		std::cout << "daha önce kayit edilmis durumda\n";
	}
}

PrinterPtr PrinterProvider::get_printer(const std::string& key)
{
	if(mprinter_map.contains(key))
	{
		if(!mprinter_map[key].m_printer)
        {
			mprinter_map[key].m_printer = mprinter_map[key].m_creator();
		}

		return mprinter_map[key].mprinter;
	}
	std::cout << "yanlis anahtar degeri\n";
	return {}; // return nullptr aslında bu.
}

//main.cpp
#include "html_printer.h"
#include "xml_printer.h"
#include "Pdfprinter.h"
#include "printer_provider.h"

int main()
{
    PrinterProvider::register_creator("xml",&XmlPrinter::get_instance);
    PrinterProvider::register_creator("html",&HtmlPrinter::get_instance);
    PrinterProvider::register_creator("pdf",&PdfPrinter::get_instance);

    auto p1 = PrinterProvider::get_printer("xml");
    if(p1)
    {
        p1->print("singleton is an antipattern!");
    }

    auto p2 = PrinterProvider::get_printer("xml");
    if(p2)
    {
        p2->print("necati ergin\n");
    }

    //auto x = *p1; // sentaks hatası copy'ye karşı kapatılmış

    //--------------------------------------------------------------

     PrinterProvider::register_creator("xml",&XmlPrinter::get_instance);
     PrinterProvider::register_creator("XML",&XmlPrinter::get_instance); // ikiside aynı adresi kayıt etti.

     auto p1 = PrinterProvider::get_printer("xml");
     auto p2 = PrinterProvider::get_printer("XML");

     if(p1)
     {
         p1->print("singleton is an antipattern!");
     }

     if(p2)
     {
         p2->print("necati ergin\n");
     }
}

MOLA

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

FABRIKA KONUSU
--------------
Fabrika terimi farklı farklı yerlerde farklı anlamda kullanılıyor.
Bir funcın varlık sebebi bir nesne oluşturmaksa bu fabrika func. C++ taki anlamı bu.
make_uniqe make_shared make_pair bunlar fabrika funcları. GOF taki factory method bunlarla
doğrudan ilgili değil.


Acaba bir sınıf nesnesini doğrudan ismiyle ile oluşturmakla, bir funcı sınıf nesnesi oluşturtmak amaçlı
kullanmak ile arasında nasıl bir fark olabilir ? Neden böyle birşey yapalım ?
Birsürü sebebi olabilir.

1 - Bazı durumlarda named ctor kullanmak exception handling olmayan durumlara karşı bir önlem.
	Named ctorun return değeri olmasından faydalanabilirim.Mesela return değerinden nesneyi
	oluşturup oluşturmadığını bir başarı değeri ile çağıran koda bildirebilir.

2 - Overloadingte ctor ismi sınıf ismiyle aynı olduğundan farklı overloadlar nesneyi nasıl kurduğunu
	anlatmıyor. Ama 2 named ctor kullanırsak, named ctor doğrudan ismiyle nesneyi ne şekilde kurduğunu anlatabilir.
	İsimlendirme nasıl bir strateji ile nesneyi oluşturduğunuda gösterebilir. Duruma göre ctora isim vermek
	parametre sayısınıda azaltabilir.

3 - Bazen problem function overloading mekanizmasında. Func overloading için imza farklı olmalı.
	3 tane double parametreli ctorlar olamaz ama isimlendirilmiş double parametreli ctor olabilir.

4 - Nesneyi ctor ettikten sonra tekrar bir functionu çağırıyoruz. Ctor yetmiyor yani hayata getirmeye.
	Bu C eski funclarında var.
	
5 - Duruma göre nesne oluşturacağız yada oluşturmama şansı yok.
	Ctor kodu sonuna kadar çalışırsa nesne oluşmuş demektir zaten. Named Ctor nesneyi oluşturamayadabilir.

6 - Birçok durumda nesnenin türü runtimeda belli oluyor.Burada runtime polymorizminden faydalınabilir.
	C++ta runtimeda sınıf oluşturulamaz nesne oluşturulabilir.

	birsürü madde eklenebilir


---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

GOFTAKİ FACTORY METODA BAKALIM
------------------------------
Bir sınıfın member func içinde bir nesnesi oluşturmamız gerekiyor ama oluşturacağımız nesnenin türünü biz bilmiyoruz,
bunu kalıtım yoluyla bizim sınıfımızdan elde edilecek sınıfların belirlemesini istiyoruz.

NOT : Smart pointersları kalıtımda kullanabiliyoruz.

Product
-------
Factory metodun oluşturacağı nesnelerin interfaceini tanımlıyor.

Concrete Product
----------------
Product interface'ini implemente eden sınıflar.

Creator
-------
Factory metodu declere eden sınıf.Creator aynı zamanda default concere object döndüren factory metodun default
implementasyonunu tanımlar.

Concrete Creator
----------------
Concrete product instantiate eden factory metodu override eder.

1:54 te hızlıca bir örnek gösterdi yazmadım
 
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

Creator sınıfı person olsun.

//person.h
class Person{
public:

	virtual ~Person() = default;

	void travel();				//operation bu

	virtual CarPtr create_car() = 0;	// travel bir nesne oluşturacak ama o nesne factory method tarafından oluşturlacak
						// burası factory method
};


//person.cpp
#include "person.h"
#include "car.h"

void Person::travel()
{
	CarPtr ptr = create_car();
	ptr->start();
	ptr->run();
	ptr->stop();
}


//car.h
#include <memory>

class Car{
public:
	virtual ~Car() = default;
	virtual void start() = 0;
	virtual void run() = 0;
	virtual void stop() = 0;
};

using CarPtr = std::unique_ptr<Car>;


//CONCRETE CREATORLERİ YAZIYORUZ

//student.h  // concrete creator
#include "person.h"

class Student : public Person{
public:
	Student();
	~Student();
	virtual CarPtr create_car()override;
};


//manager.h   //concrete creator
#include "person.h"

class Manager : public Person{
public:
	Manager();
	~Manager();
	virtual CarPtr create_car()override;
};


CONCRETE PRODUCTLARIDA YAZALIM

//mercedes.h
#include "car.h"

class Mercedes : public Car{
public:
	virtual void start()override;
	virtual void run()override;
	virtual void stop()override;
};


//renault.h
#include "car.h"

class Renault : public Car{
public:
	virtual void start()override;
	virtual void run()override;
	virtual void stop()override;
};


//manager.cpp
#include "manager.h"
#include "mercedes.h"
#include <iostream>

Manager::Manager()
{
	std::cout << "Manager nesnesi olusturuldu\n";
}

Manager::~Manager()
{
	std::cout << "Manager nesnesi yok edildi\n";
}

CarPtr Manager::create_car()
{
	return std::make_unique<Mercedes>();
}


//student.cpp
#include<iostream>
#include "student.h"
#include "renault.h"

Student::Student()
{
	std::cout << "Student nesnesi olusturuldu\n";
}

Student::~Student()
{
	std::cout << "Student nesnesi yok edildi\n";
}

CarPtr Student::create_car()
{
	return std::make_unique<Renault>();
}



//mercedes.cpp

#include "mercedes.h"
#include <iostream>

Mercedes::Mercedes()
{
	std::cout << "Mercedes olusturuldu\n";
}

Mercedes::~Mercedes()
{
	std::cout << "Mercedes yol edildi\n";
}

void Mercedes::start()
{
	std::cout << "Mercedes has started!\n";
}

void Mercedes::run()
{
	std::cout << "Mercedes is running!\n";
}

void Mercedes::stop()
{
	std::cout << "Mercedes has stopped!\n";
}


//renault.cpp

#include "renault.h"
#include <iostream>

Renault::Renault()
{
	std::cout << "Renault olusturuldu\n";
}

Renault::~Renault()
{
	std::cout << "Renault yol edildi\n";
}

void Renault::start()
{
	std::cout << "Renault has started!\n";
}

void Renault::run()
{
	std::cout << "Renault is running!\n";
}

void Renault::stop()
{
	std::cout << "Renault has stopped!\n";
}

//main.cpp

#include "student.h"
#include "manager.h"

void game(Person&p)
{
	p.travel();
}


int main()
{
	Manager m;
	game(m);

	Student s;
	game(s);

}

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

BU ÖRNEKTE PARAMETRİK FABRİKA YAPILACAK
---------------------------------------
Bir application sınıfı var.Bu creatorun yerinde.
Değişen şu, goftaki patern olsa biz application sınıfından kalıtım yoluyla app sınıfından bir sınıf elde edeceğiz,
concrete creater elde edeceğiz. Concrete creater, create funcını override edecek.

Aslında runtimeda hangi dökümanın oluşturulacağını belirleyecek bir araç lazım. Bu özellikle
kapalı sınıf hiyerarşileri için çok pratik.

Kısaca burada hazır bir kod vardı hoca onu açıkladı.









*/
