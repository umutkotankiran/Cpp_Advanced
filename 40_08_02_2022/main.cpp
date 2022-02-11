/*

Clone ve Create functionlarının unique ptr ile implementasyonu.

Polimorfik kopyalama için kullanılan idiyomun ismi clone.
Bİr nesnenin dinamik türünü koruyarak doğrudan copy mümkün değil.

Polimorfik kopyalamayı unique ptr üzerinden yapınca dikkat edilen bir iki nokta olacak.
Sınıfın sanal functionları private olabilir. Hiçbir problem yok hata non virtual interface
idiyomu sanal sınıfları private yapmak ile ilgili.

Covariant return type. Nromalde bir funcı return ederken taban sınıfın sanal funcıyla
aynı return türüne sahip olmalı. Bunun istisnası covariance.

DİKKAT!!!!
Covariance 'ın unique ptr de olması söz konusu değil.
Yani taban sınıfın taban funcı unique_ptr<A> ise override eden func unique_ptr<B>
dönemez.Smart Ptr larda bu yok.

Covariance hatırlatması Temel C++
---------------------------------
class Base
{
public:
	virtual ~Base();
};

class Der : public Base
{
public:
	
};


class A
{
public:
	virtual Base* func();
	//virtual Base& func();
};

class B : public A
{
public:
	Der* func()override;	 // covariance
	//Der& func()override;	 // covariance
}

Return türü, referans yada pointer olmak zorunda.



Unique_Ptr ile yapılamayan örnek
--------------------------------

class Base
{
public:
	virtual ~Base();
};

class Der : public Base
{
public:

};


class A
{
public:
	virtual std::unique_ptr<Base> func();
};

class B : public A
{
public:
	virtual std::unique_ptr<Der> func()override;	 // Sentaks Error. Covariance değil.
}

-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------

UNIQUE PTR İLE CLONE İDİYOMU REPOYA YÜKLEDİM.
CLONE1

-------------------------------------------------------------------------------------------

===========================================================================================================================
===========================================================================================================================
===========================================================================================================================
===========================================================================================================================
===========================================================================================================================

ADVANCED EXCEPTION HANDLING
---------------------------
C++11 de dile bazı araçlar eklendi. Bunlar çok önemli araçlar. Birbiriyle mantıksal ilişki içerisindeki öğeler exception
header fle içerisinde tanımlanan exception_ptr denen tür. Pointer like bir sınıf. Bazı noktalarda implementasyonu nerleyiciye
bağlı. Bu nullable bir type.Yani pointer olmasada nullptr semantiği var. ÖR: Logic bağlamda logic kontrol yapınca değeri
nullptr ise false, değilse true olarak yorumlanır.unique_ptr, shared_ptr bunlar nullable type.Optionalde böyle.

exception_ptr tek başına bir işe yaramıyor.Temel varlık nedeni diğer iki funcla birlikte ortaya çıkacak.
Biri rethrow_exception diğeride current_exception.Hatta birde fabrika func var. make_exception_ptr
Exception handling kullanmanın yasak olduğu yerlerde zombie objectler kullanılıyor.

1 - exception_ptr
2 - rethrow_exception
3 - current_exception
4 - make_exception_ptr

---------------------------------------------------------------------------------------------------------------------------

std::exception_ptr
------------------
Bu nullable bir type ve default ctor edersek nullptr değerinde.
Yakalanmış ve current exceptionla elde edilmiş bir exception nesnesini gösteriyor.Başka bir funca geçilebilir.

Başka bir threadde olabilir.Tekrar gönderilebilir ve bir catch close u ile yakalanabilir.
Otomatik olarak hiçbir türe dönüşmüyor.Sadece nullable type olduğundan bool türüne dönüşüyor.
Eğer bir exception nesnesini gösteren exception_ptr varsa, o nesne onu gösteren tüm exception ptr lerin
hayatı bitene kadar hayatta kalmak zorunda.


Rethrow_exception
-----------------
[[noreturn]] void rethrow_exception(std::exception_ptr p);

Öncesinde yakalanmış bir hata nesnesi ama onun bir kopyasıda olabilir.Bir kopyalama yapılıp yapılmadığı derleyiciye bağlı.

Bir current_exception çağırınca exception_ptr elde ediyoruz, bu exception ptr kopyalanabilir ama eğer herhangibir copy sırasında
dinamik bellek alanı kullanılıyorsa ve orada eğer operator new fail olursa bad_alloc sınıfı türünden exception throw ediyor.
Ama copy nedeniyle bir exception throw ederse o zamanda hangi türden exception throw ederse o türden.Ama copy olayı tamamen
Implementation Defined.

Noreturn olarak tanımlanmış.Bu bir attribute.
BIR EXCEPTION NESNESINI YAKALAYIP O EXCEPTIONU BAŞKA BIR SCOPEA AKTARIP ORADAN TEKRAR GÖNDERMEK IÇIN BU ARACI KULLANIYORUZ.


Current_exception
-----------------
Bir exception yakalandığında bize o exceptiona işaret eden bir exception_ptr gönderiyor ama
bu exception_ptr derleyiciye bağlı bir tür.Current_exception bir noexcept function.

---------------------------------------------------------------------------------------------------------------------------
#include <exception>
#include <iostream>

int main()
{
	using namespace std;
	exception_ptr ptr;
	
	cout << (ptr ? : "Null Degil" : "null"); // nullptr
	
	if(!ptr) // null ise girer
	{

	}
}

Neler yapılabilir?
Ya boş herhangibir exceptionu göstermiyor yada bir exception nesnesini gösteriyorki o exception nesnesi aslında
yakalanmış ve rethrow edilebilir.Dolayısıyla exception_ptr türünden bir değişkenle, yakalanmış bir exception nesnesini
exception nesnesiyle capture edersek onu başka bir funca gönderip o func içinde de rethrow_exceptionla exceptionu throw
edebiliriz.


#include <exception>
#include <iostream>
#include <vector>
void func(std::exception_ptr ptr) // Burada exception_ptr ile olsa referans semantiğimi yoksa copy semantiği mi olduğu derleyiciye bağlı.
{								// Belirli nedenlerden ötürü yapmışlar bunu

	try
	{
		std::rethrow_exception(ptr);  // Exception_ptr nin tuttuğu exceptionun rethrow edilmesi için bu funcı çağırmalıyız.
									  // Eğer ptr boş ise Undefined Behavior.
	}
	catch(const std::out_of_range& ex)
	{
		std::cout << "hata yakalandi...\n";
		std::cout << ex.what() << '\n';
	}
}

using namespace std;
int main()
{
	vector<int> ivec(10);
	
	try{
		auto x = ivec.at(20);
	}
	catch(const std::exception& ex)
	{
		std::cout << "exception caught: " << ex.what() << '\n';
		
		//current_exception();    // bir try veya catch bloğu içinde çağırırsak return değeri az önceki exception_ptr türünden
								// hata yakalanmışsa bize o hata nesnesini gösteren bir exception_ptr veriyor
		
		auto eptr = current_exception(); // yakalanan exception nesnesi yoksa current exceptionun return ettiği
										// exception ptr boş olacak. Şimdi biz bunu başka bir functa throw edeceğiz.
										// bunu funca göndererek yapacağız.
		
		func(eptr); // Funca gönderildi

	}
}

Burada bir exceptionun bir scopetan başka bir scopea aktarılması ve dinamik türünün korunarak oradan tekrar rethrow edilebilmesi.
Üstelik bu diğer funca aktarılan exception başka bir thread tarafındanda çalıştırılmış olabilir.Threadlerden exception
gönderilmesi için kullanılan mekanizmanında özünü oluşturuyor.

BAZI GARANTILER VAR
Exception ptr nesneleri eşitlik sınamasına sokulabilir.
2 exception ptrde boşsa eşit olacak, ikiside aynı hata nesnesini gösteriyorsa yine true.
Exception ptr türünden diğer türlere sınıflara dönüşüm yok.
exception_ptr, herhangibir exception sınıfı türünden değil.


---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------


EXCEPTION PTR ELDE ETMEK IÇIN CURRENT_EXCEPTION ÇAĞIRMAMIZ GEREKIYOR AMA CURRENT EXCEPTION ÇAĞIRMADAN DA MAKE_EXCEPTIN_PTR
FABRIKA FUNC ILE BIR EXCEPTION_PTR ELDE EDEBILIRIZ.

Bildirim
template<typename Ex>
std::exception_ptr make_exception_ptr(Ex ex);


#include <exception>
#include <iostream>
#include <vector>

void func(std::exception_ptr ptr)
{
	try
	{
		std::rethrow_exception(ptr);

	}
	catch(const std::out_of_range& ex)
	{
		std::cout << "hata yakalandi...\n";
		std::cout << ex.what() << '\n';
	}
}

int main()
{
	make_exception_ptr(std::out_of_range{"aralik disi deger"});		// Bu bize bir exception_ptr değer döndürecek
}

YUKARIDAKI KOD ASLINDA AŞAĞIDAKI KOD ILE KABACA AYNI ANLAMDA

try{
	throw ex;
}
catch(...)
{
	return std::current_exception();
}


---------------------------------------------------------------------------------------------------------------------------

ÇOK ÖNEMLİ !!!!!!

try{
	throw std::out_of_range{"range hatasi"};
}
catch(const std::exception& ex)
{
	std::cout << "hata func icinde takalandi : " << ex.what() << '\n';
	throw ex; // Bu aşağıdaki ile aynı değil.Burada bir kopyalama var ve bunda dinamik tür korunmuyor.Static tür esas alınır.

	throw; // Burada ise exception nesnesinin kendisini gönderiyoruz.
}


ÖR:
void func()
{
	try{
		throw std::out_of_range{"range hatasi"};
	}
	catch(const std::exception& ex)
	{
		std::cout << "hata func icinde yakalandi : " << ex.what() << '\n';

		throw ex; // Burada kopyalama olur ve dinamik tür kaybedilir.Bu sebeple en tepedeki exception nesnesi gönderilir.
				  // Aşağıda std::exception yakalandıya girer.Burada bir object slicing oluyor.
		
		throw;   // Burada nesnenin kendisi throw edilir ve dinamik tür korunur.
				 // Bu durumda out of range hatasına girer.
	}
}

int main()
{
	try
	{
		func();
	}
	catch(const std::out_of_range&)
	{
		std::cout << "hata yakalandi (std::out_of_range)\n";
	}
	catch(const std::exception&)
	{
		std::cout << "hata yakalandi (std::exception)\n";
	}
}
---------------------------------------------------------------------------------------------------------------------------

RETHROW STATEMENT CATCH BLOĞU İÇİNDE OLACAK DİYE BİRŞEY YOK.

void foo()
{
	std::cout << "foo cagrilir\n";
	throw;
}

void func()
{
	try{
		throw std::out_of_range{"range hatasi"};
	}
	catch(const std::exception& ex)
	{
		std::cout << "hata func icinde yakalandi : " << ex.what() << '\n';
		foo();
	}
}

int main()
{
	try
	{
		func();
	}
	catch(const std::out_of_range&)
	{
		std::cout << "hata yakalandi (std::out_of_range)\n";
	}
	catch(const std::exception&)
	{
		std::cout << "hata yakalandi (std::exception)\n";
	}
}

Hata functa yakalandı. foo çağrıldı sonra catch içerisinden sonra hata rethrow edildi.
rethrow edilen hatada main içindeki catch içinde yakalandı.

---------------------------------------------------------------------------------------------------------------------------


STD::TERMINATE ÇAĞRILDIĞI DURUMLAR
----------------------------------
Defaultta abort çağrılır ama set_terminate func ile terminate davranışını değiştirebiliriz

using terminate_handler = void(*)(void);
terminate_handler set_terminate(terminate_handler);  // Func bildirimi

1 - Nothrow garantisi veren functionların exception throw etmesi durumunda terminate eder.Destructor mesela
2 - Throw edilen exceptionun yakalanamaması.Uncaught durumunda terminate eder.
3 - throw; statement bir catch bloğundan çağrılırsa yakalanan exception rethrow eder. Eğer doğrudan çağrılırsa,
	ortada yakalanan exception olmadığından terminate çağrılacak.
	
	void f()
	{
		throw;
	}
	
	int main()
	{
		try{
			f(); // terminate çağrılır.
		}
		catch(...)
		{
			std::cout << "exception caught\n";
		}
	}

Exception amid etmesi demek dışarı çıkması demek. Yani Destructor içinde exception throw edilebilir ama kendi içinde
halledilmesi gerekiyor bunun.

---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
FUNCTION TRY BLOCK - TEMEL C++
------------------------------

void func()
{
	try
	{
		//code
	}
	catch(int)
	{

	}
}

Yukarıdaki gibi yazmakla aşağıdaki yazım şekli aynı

void func(int x)  // Burada function try block var. Func Parametresini catch içinde kullanabiliriz.
try
{
	throw std::runtime_error{"hata"};
}
catch(const std::exception &ex)
{
	std::cout << "exception caught\n";
	return x * x; // x burada da kullanılabilir.
}

int main()
{
	auto i = func(23);
	std::cout << "i = " << i << '\n';
}

1 - func parametresi try catch bloklarında kullanılabilir
2 - functionda hata oluşutsa catch yakalayacak.
Bu normal functionlarda çok sık kullanılmıyor.Daha çok constructorlarda kullanılıyor.

---------------------------------------------------------------------------------------------------------------------------

KABACA ÖRNEK

class Myclass{};

Myclass func(Myclass x)
try{
	throw std::runtime_error{"hata"};
	return x;
}
catch(const std::exception &ex)
{
	std::cout << "exception caught... : " << ex.what() << '\n';
	return x;
}

func(Myclass{}); // Gibi bir çağrıda eğer argümandan parametreye kopyalama işlemi sırasında hata oluşursa function try bunu kontrol etmez.
				 
---------------------------------------------------------------------------------------------------------------------------

UB ÖRNEĞİ

int func()
try{
	throw 1;
}
catch(int)
{
		// catch bloğu içinde return statement yok. Undefined Behavior
}

---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------

ASIL OLAY CONSTRUCTORLARDA

Memberdan gönderilen exceptionu yakalamak istiyoruz

class Member
{
public:
	Member()
	{
		std::cout << "Member default ctor\n";
		throw 1;
	}
	
};

class Nec
{
public:
	Nec()
	{
		try
		{
		}
		catch(int)
		{
			std::cout << "exception caught\n";   // Yukarıdaki hata burada yakalanmaz !!!!!!!!!!
		}
	}
private:
	Member mx;
};

int main()
{
	Nec mynec; //hata yakalanamadığı için terminate etti.
}

Modern C++ öncesinde, bir ihtiyaç oluştuğu için loglama yapacağız yada exceptionu translate edeceğiz.
translate o exceptionu alıp başka bir exception göndereceğiz demek.
Bu durumda func try blok kullanıyoruz.

---------------------------------------------------------------------------------------------------------------------------

ÖR:

class Member
{
public:
	Member(int)
	{
		std::cout << "Member default ctor\n";
		throw 1;
	}

};

class Nec
{
public:
	Nec() try : mx{12}
	{
	
	}
	catch(int)
	{
		std::cout << "exception caught\n"; 
		
		//// ... // Burada hiçbirşey yapılmazsa exception rethrow eder.

		rethrow;  // Burada da yukarıdaki gibi exception rethrow eder. Kullansakta kullanmasakta rethrow eder
				  // dilin kuralı bu.

		throw std::runtime_error{"member exception"}; // Burada istediğimiz başka bir eceptionda gönderebiliriz
	}

private:
	Member mx;
};

int main()
{
	Nec mynec;
}

---------------------------------------------------------------------------------------------------------------------------

ÖR:

İki struct nesnesi var ikiside aynı türden hata gönderme ihtimalinde.
Burada hatayı nereden gönderileceğini nasıl bileceğiz.

#include <stdexcept>
#include <iostream>
#include <cassert>

struct X {
	X(int)
	{
		throw std::runtime_error("X error");
	}
};

struct Y {
	Y(int)
	{
		throw std::runtime_error("Y error");
	}
};

class Nec {
private:
	X mx;
	Y my;
	enum Tracker { NONE, FIRST, SECOND };
public:
	Nec(Tracker tracker = NONE)	try	: mx((tracker = FIRST, 1)) , my((tracker = SECOND, 2))   // virgül operatörü sequence point oluşturur.
	{																						// ürettiği değerde sağ operand tır.
		assert(tracker == SECOND);
		// ... constructor body ...
	}
	catch (const std::exception& e)			// mx yada my hata verirse bu bloğa girecek.2 side hata vermezse catch bloğuna girmeyecek akış.
	{
		if (tracker == FIRST) {				// buraya girerse X exception throw etmiş
			std::cout << "X threw: " << e.what() << '\n';
		}
		else if (tracker == SECOND) {		// buraya girerse Y exception throw etmiş
			std::cout << "Y threw: " << e.what() << '\n';
		}
		throw;
	}
};

int main()
{
	try {
		Nec nec;
	}
	catch (const std::exception &e) {
		std::cout << "exception caught: " << e.what() << std::endl;
	}
}

---------------------------------------------------------------------------------------------------------------------------

BURADA ORTAK KODUN BİR YERDE TOPLANMASI TEMASI VAR.

#include <iostream>

class a_exception {
	//...
};

class b_exception {
	//...
};

class c_exception {
	//...
};


void handle_exception()
{
	try {
		throw; //rethrow statement
	}
	catch (const a_exception&) {
		std::cout << "a_exception is handled\n";
	}
	catch (const b_exception&) {
		std::cout << "b_exception is handled\n";
	}
	catch (const c_exception&) {
		std::cout << "c_exception is handled\n";
	}
}

void f1()
{
	throw b_exception{};
}

void f2()
{
	throw c_exception{};
}

void f3()
{
	throw a_exception{};
}


int main()
{
	try {
		f1();
	}
	catch (...) {
		handle_exception();
	}

	std::cout << "[1] main is running\n";


	try {
		f2();
	}
	catch (...) {
		handle_exception();
	}
	std::cout << "[2] main is running\n";


	try {
		f3();
	}
	catch (...) {
		handle_exception();
	}
	std::cout << "[3] main is running\n";
}














































































*/
