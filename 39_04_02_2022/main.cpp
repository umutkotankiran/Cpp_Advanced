/*

Bir Code review yapıldı.

C++ TEKNIKLERINE DEVAM EDECEĞIZ.

SCOPE GUARD
-----------
Bunu implemente eden standart library yok.Sıksık kullanılıyor. 
Öneriler verilmiş birden fazla.scope_exit var ama standart değil henüz.

Bu RAII nin tipik örneği. Scopetan çıkınca bir funcın çağrılmasını sağlamak istiyoruz.
İdeali bunun yapılmaması tabi. Func call yapılmasındaki neden bir kaynağın geri verilmesi,
bazende exceptionla ilgili strong exception garantisi vermek için bazı işlemlerin yapılması.

gsl.h diye bir library hoca indirmiş. Microsoftun var googledan bulunabilir.

Bazı durumlarda exception throw edildiğinde ilgili func calla girmeden program sonlanır ama
bu yapıda ilgili func exception throw etsede çağrılıyor.

void func()
{

	std::cout << "main basladi\n";

	//gsl::final_action  //Bu bir sınıf şablonu ve bzden bir callable istiyor.
						//Callable ı sarmalıyor ve dtoru o callable ı çağırıyor.
	

	auto f = []{
		std::cout << "Necati Ergin\n";
	};

	// gsl::final_action<void()> x{f}; //normalde böyle.Bir hata olabilir.
	gsl::final_action x{ f }; //ctad
	// finally(f); // burada ctad yok. Ctad olmadan da bunu kullnabiliriz.

	if(1)
	{
		gsl::final_action a{f};
	}

	std::cout << "main devam ediyor\n";
}

C++23'te standart olabilir.

main basladi
Necati Ergin
main devam ediyor.

a değişkeni için scope sonunda dtor çağrıldı. Çağrılan dtor sarmaladığı callable çağırdı


----------------------------------------------------------------------------------------------------

TEMPLATE 
Tam değil ama benzer birşey yazıyoruz.

#include <utility>

template <typename Func>
class scope_guard{
public:
	scope_guard(Func f)noexcept : m_f(f) { }

	~scope_guard()
	{
		if(m_call_flag)
			m_f();
	}

	scope_guard(const scope_guard&& other ) noexcept 
	: m_f{std::move(other.mf)}, m_call_flag{other.m_call_flag}
	{
		other.m_call_flag = false;
	}

	void release()noexcept
	{
		m_call_flag = false;
	}

	scope_guard(const scope_guard&) = delete;
	scope_guard& operator=(const scope_guard&) = delete;
private:
	Func m_f;
	bool m_call_flag{ true }; // bazı senaryolarda dtor çağrılmasına rağmen func çağrılmasını bloke etmek için
};								// bu değişken kullanılıyor.

template <class Func>
scope_guard<Func>finally(const Func&f)  // return değeri autoda olabilir.
{
	return scope_guard<Func>{f};
}


#include <iostream>

int main()
{
	std::cout << "1...\n";

	auto f = []{ std::cout << "deneme\n";};

	{

		//scope_guard<decltype(f)>sg{f};
		//auto sg = finally(f);  // finally ile de olur 
		
		scope_guard sg{f}; // Ctad ile böylede olur
	}


	std::cout << "2...\n";
}

======================================================================================================
======================================================================================================
======================================================================================================
======================================================================================================
======================================================================================================
======================================================================================================


SINIFLARIN PRIVATE BÖLÜMÜNÜN GİZLENMESİ
---------------------------------------
Sınıfın private bölümünü header fileda göstermek istemiyoruz.
Neden ? 
Bizim header fileı 10larca dosya include edebilir. Bu dosya include edildiğinde
eğer bizim header fileda yada include ettiğimiz bazı header file larda bir değişiklik
olduğunda başlık dosyasını include eden tüm kodların recompile edilmesi gerekiyor.

class Myclass
{

private:
	// private func
	T x;
	U y;
};

Birinci problem dependancy.
U ve T için header filelar include ediyorum.Bunları include edince onları include edenleride include ediyoruz.
Bu başlık dosyalarda bir değişiklik olunca bir recompile edilmesi gerekiyor.

- Recompile edilmemesi runtime veya link hatası olabilir
- Birçok başlık dosyasını include ettiğim için başlık dosyasından gelen kodlarda derlendiği için 
  compile time hızı düşüyor.100 lerce header file varsa çok önemli oluyor.Build zamanı 7-8 saat artabilir.
  Diğer header filelara bağlı hale geliyoruz.

Problemin büyüğü dependancy kısmı. Diğer headerlardaki değişikliğin benimde compile etmeme ihtiyaç oluşturması
İkinci problem compile timeın uzaması

-----------------------------------------------------------------------------------------------------

HANGI DEĞIŞIKLIKLERDE BIR DOSYANIN BAŞTAN DERLENMESI GEREKEBILIR.

//necati.h
class Myclass{
public:
	void f1();
	void f2();
	void f3();
	
	// void f4();  f4 eklenirse recompile gerekmiyor.
	// virtual void f5(); // Kesinlikle recompile edilmeli. ABI deniyor buna
private:
	//T x;
	//U y; //Elemanların yer değiştirmeside recompile
	//int z; // sonradan ekledik diyelim. Yine recompile etmek gerekti.Eleman silmede de recompile gerekli
};				

Applications Programming interface    API

Applications Binary interface		ABI  
bu derleyici için olan interface.Class layoutu, elemanlar bellekte nasıl olacak, virtual table implementasyonu...
Sınıfta değişiklk yapınca bu binary interface bozuluyor.Bu derleyici için olan interface.
Sınıfın private tarafını gizlediğimizde sınıfın  private elemanı hiç olmadığından(özellikle bu bölümü gizlersek)
bir değişiklik yapıldığında bir compile gerekmiyor.

virtual func eklenince
eleman eknenince / silince / yer değiştirince recompile gerekli.
Member func ekliyorsak static/nonstatic, birde global func eklersek problem yok onunda dışındaki tüm durumlar sorunlu

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

3 AYRI TEKNIKLE SINIFIN PRIVATE KISMI GIZLENECEK

1 - Kalıtım
2 - Pimple
3 - ...

//student.h
#include<string>
#include<vector>
//other include

class Student
{
public:
	Student(std::string name, std::string surname);

	void add_grade(int);
	void print()const;
private:
	std::string m_name;
	std::string m_surname;
	std::vector<int> m_grades;
	//other private members
	//other private functions
}

//Student.cpp
#include "student.h"

Student::Student(std::string name, std::string surname) : m_name{std::move(name)}, m_surname{std::move(surname)}
{

}

void Student::add_grade(int grade)
{
	m_grades.emplace_back(grade);
}

void Student::print(int)const
{
	std::cout << "name : " << m_name << " surname : " << m_surname << '\n';
	std::cout << "grades : ";
	for(auto g : m_grades)
		std::cout << g << ' ';

	std::cout << "\n";
}

//main.cpp
int main()
{
	Student s{"Burak","Akman"};

	s.add_grade(12);
	s.add_grade(45);
	s.add_grade(99);

	s.print();
}


-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

1 KALITIMLA
-----------
Buradaki olay taban sınıf abstract bir sınıf ve türemiş sınıf nesnesini, base class olan abstract sınıf
ile tutuyoruz.Böylece Private bölüm gizlenmiş oluyor.Çok iyi bir çözüm olmayabilir.

//istudent.h
#include<string>
#include<vector>
//other include

class IStudent
{
public:
	virtual ~IStudent() = default;
	virtual void add_grade(int) = 0;
	virtual void print()const = 0;
};


//student.h
#include "istudent.h"

class Student : public IStudent
{
public:
	Student(std::string name, std::string surname) : m_name{std::move(name)}, m_surname{std::move(surname)}
	{
		
	}

	void add_grade(int)override;
	virtual void print()const override;

private:
	std::string m_name;
	std::string m_surname;
	std::vector<int> m_grades;
	//other private members
	//other private functions
};


//student.cpp
#include "student.h"
#include <iostream>

void Student::add_grade(int grade)
{
	m_grades.emplace_back(grade);
}

void Student::print() const
{
	std::cout << "name : " << m_name << " surname : " << m_surname << '\n';
	std::cout << "grades : ";
	for(auto g : m_grades)
		std::cout << g << ' ';

	std::cout << "\n";
}


//factory.h
#include "istudent.h"

IStudent* create_student(std::string name, std::string surname);  // Olay burada. new student nesnesi Istudent ile tutulmuş
 

//factory.cpp
#include "factory.h"
#include "student.h"

IStudent* create_student(std::string name, std::string surname)
{
	return new Student{std::move(name), std::move(surname)};	
}


//main.cpp
#include "factory.h"

int main()
{
	auto p = create_student("Harun","Bozaci");

	p->add_grade(12);
	p->add_grade(45);
	p->add_grade(99);

	p->print();

	delete p;
}


-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

2. PIMPL İDİYOMU İLE
--------------------
Mülakatlarda çok soruyorlar
2 tip implementasyon var. 
Biri klasik olan o anlatılmayacak, sınıfa eleman olarak raw ptr konuluyor ve sonra delete edilme problemi var.


//student.h
#include<string>
#include<memory>

class Student
{
public:
	Student(std::string name, std::string surname);
	Student(const Student&);
	Student& operator=(const Student&);

	Student(Student&&);
	Student& operator=(Student&&);


	//Bunu yazmaksak aşağıda yazdığım hatayı alırız.O yüzden eklendi.Ama inline tanımlamadık.cpp dosyasında tanımlandı
	~Student(); 
	
	void add_grade(int);
	virtual void print();

private:
	class pimpl; // şuanda incomplete type durumunda.Bir nested type için forward decleration. Globalda değil.

	std::unique_ptr<pimpl>p;  // bu haliyle bir student nesnesi oluşturmak istediğimde sentaks hatası olur.
							  // Bu sınııfn dtoru derletyici tarafından default edilecek.Derleyicinin yazdığı dtor
								// nonstatic public inline func. Böyle olduğunda dtor elemanların dtorunu çağırıyor
								// unique_ptr nin dtoru çağrıldığında default delete buradaki pimple nesnesini delete 
								// etmeye çalışıyor ve incomplete type olduğundan sentaks hatası oluyor.
								
};

//student.cpp
class Student::pimpl {
public:
	pimpl(std::string name, std::string surname) : m_name{ std::move(name) }, m_surname{ std::move(surname) }
	{

	}
	void add_grade(int grade)
	{
		m_grades.emplace_back(grade);
	}
	void print()const
	{
		std::cout << "name : " << m_name << " surname : " << m_surname << '\n';
		std::cout << "grades : ";
		for (auto g : m_grades)
			std::cout << g << ' ';
		std::cout << "\n";
	}
private:
	std::string m_name;
	std::string m_surname;
	std::vector<int> m_grades;
};

Student::~Student()
{
	std::cout << "dtor\n";
}

HOCAYA SOR : NEDEN INLINE OLARAK YAZMADIĞIMDA HATA ALMIYORUM?

Student::Student(std::string name, std::string surname) : 
	p{std::make_unique<pimpl>(std::move(name),std::move(surname ))} { }

Student::Student(const Student& other) : p{ new pimp{*other.p} } {  }  // Kopyamak için

Student& Student::operator=(const Student& other)
{
	p.reset(new pimpl{ *other.p });
	return *this;
}

Student::Student(Student&&) = default;
Student& Student::operator=(Student&&) = default;

void Student::add_grade(int grade)
{
	p->add_grade(grade);
}

void Student::print()const
{
	p->print();
}


//main.cpp
#include <iostream>
#include "student.h"

int main()
{
	{
		Student s{"murat","hepeyiler"};

		s.add_grade(34);
		s.add_grade(89);
		s.add_grade(100);

		s.print();

		auto s2 = s;
		s2.print();

	}
	std::cout << "main devam ediyor\n";
}

UYARILAR
---------
Dtorun durumu : Dtoru class içinde bildirmeseydik, derleyici inline yapacaktı ve incomplete type olan
pimpl delete edilmeye çalışılacaktı bu da sentaks hatası olacaktı. Yani bildirim sınıf içinde
default edilirken tanım yapılacak yada default edilecek.

Constluk Problemi : Çözümü var ama standart değil. propagate_const.cons member func içinde pimlp ptr kullanarak
		    const olmayan member funca çağrı sentaks hatası değil. hata olması için propagate_const kulalnacağız.
		    bu class konusunda detaylı görülen bir konuydu. Const func içinden doğrudan nonconst
 		    bir funca call yapılamıyordu. Gizli pointer nesnesinden ötürü.
		    Ama const olmayan bir değişken tanımlayarak obj.func() gibi bir çağrı const func içinden
		    yapılabilir.


-----------------------------------------------------------------------------------------------------

FAST PIMPLE IDIOM
Dinamik bellek yönetimi yapmıyor.
Private bölümde implementasyon nesnemizin bellek alanını bir veri elemanı olarak kullanacağız.

Not : Aligned storage ayrı bir derste anlatılacak.

//logger.h
#pragma once
#include <string>
#include <type_traits>

class Logger {
public:
	Logger();
	~Logger();
	void log(const std::string& str);
	//other functions
private:
	constexpr static std::size_t size = 1024;
	std::aligned_storage_t<size, std::alignment_of_v<std::max_align_t>> impl{};
};


//logger.cpp
#include "logger.h"
#include "placement.h"
#include <fstream>
#include <iostream>

class LoggerImpl {
public:
	void log(const std::string& str)
	{
		std::cout << "message : " << str << std::endl;
		ofs << str << std::endl;
	}
private:
	std::ofstream ofs;
};

Logger::Logger()
{
	static_assert(sizeof(impl) >= sizeof(LoggerImpl));
	//new(&impl)LoggerImpl;
	placement_new<LoggerImpl>(&impl, sizeof LoggerImpl);
}

Logger::~Logger()
{
	// reinterpret_cast<LoggerImpl*>(&impl)->~LoggerImpl();
	//placement_cast<LoggerImpl>(&impl)->~LoggerImpl();
	placement_delete<LoggerImpl>(&impl);
}

void Logger::log(const std::string &str)
{
	//reinterpret_cast<LoggerImpl*>(&impl)->log(str);
	placement_cast<LoggerImpl>(&impl)->log(str);
}


NOT : Herhangibir nesne türünden adresi char * a dönüştürüp sonra tekrar o nesne türüne dönüştürmek geçerli. UB değil.


//placement.h		// daha sade olsun diye yapıldı yoksa ~Loggerdaki gibide yapılabilirdi.
#pragma once

#include <cstddef>

template<typename T>
void placement_new(void* buffer, std::size_t buffer_size) 
{
	new(buffer) T();
}

///casts a given address to a pointer to type T

template<typename T>
T* placement_cast(void* buffer) 
{
	return reinterpret_cast<T*>(buffer);
}

///call the destructor of type T at a given address
template<typename T>
void placement_delete(void* buffer) 
{
	placement_cast<T>(buffer)->~T();
}


//main.cpp

#include "logger.h"
int main()
{
	Logger mylogger{};

	mylogger.log("necati ergin");
}

*/
