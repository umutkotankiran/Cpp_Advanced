/*

Reference Quailifers ta kaldık.Modern C++ ta eklenen ve daha sonraki standartlarda
genişletildi. Referans Niteleyicileri.

Sınıfın üye funclarını niteliyorlar. Çağrılmasını sınıf nesnesinin r veya lvalue olmasına bağlıyordu.
Bu yolla function overloading yapabiliyorduk. Neden yapalım? mesela bazı funcların sağ taraf değeri olan
sınıf nesneleri için çağrılmalarını engellemek. Sol taraf ve sağ taraf değeri olan sınıf
nesneleri aynı funcın farklı kodlar oluşturmak yani overloading yapmak.
Sınıfın member funcları için kullanılabilir. Global funclar için kullanılmıyor.


ÖNEMLİ KURALLAR
---------------
1 - Reference quailifer kullanırsak, reference qualifier olmayan bir functionu overload olarak yazamayız.

ÖR:
class Myclass{
public:
	void func(int);  // Sentaks hatası oldu. Sebebi Referans olmayan overload kabul etmiyor.
	void func(int)&;
	void func(int)&&;
};

Sebebi Referans olmayan overload kabul etmiyor !!

2 - Sağ taraf ve sol taraf için farklı kodları kullanılması örneğin veri elemanını get functionlar için
önemli olabiliyor.

struct Person{
	
	const std::string &getname()const // duruma göre dangling reference olma tehlikesi vardı.copy maliyetli yoktu
	{
		return ms;
	}
	std::string ms{"Ali Kalan"}; // SSO konusuna girildi çok kısa.Small string optimization
};

Person create_person()
{
	Person p;
	//..
	return p;
}

int main()
{
	for(auto c : create_person().getname())
	{
		std::cout.put(c); // DANGLING REFERENCE DEMİŞTİK.UB ZATEN
	}
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------

BUNUN ÇÖZÜMÜ

Bu da modern bir idiyom artık.

struct Person{

	const std::string &getname()const & 
	{
		return ms;
	}

	std::string getname() &&
	{
		return std::move(ms);
	}

	std::string ms{"Ali Kalan"}; // SSO konusuna girildi çok kısa
};

Person create_person()
{
	Person p;
	//..
	return p;
}

int main()
{
	for(auto c : create_person().getname())
	{
		std::cout.put(c); //GEÇERLİ. UB YOK ARTIK.
	}
}

create_person() ın return değeri person yani sağ taraf değeri, sağ  taraf değeri için && parametreli overload çağrılacak.
SOrun yok artık

--------------------------------------------------------------------------------------------------------------------------------------------------------------

Bazı sorular soruldu bunlar cevaplanıyor.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

Belki yazılabilir. Bir türün lvalue mu r value mu gl value mu olduğu nasıl anlaşılır

int main()
{
	int x = 10;

	if constexpr(std::is_reference_v<decltype((x + 5))>){
		//Glvalue lvalue veya x value bunlarda int& veya int&& yani referans türü :D:D
	}
	
	if constexpr(!std::is_lvalue_reference_v<decltype((x + 5))>){
		//rvalue. lvalue referans değilse ya rvalue reference yada referans değil.
	}
	
	if constexpr(std::is_reference_v<decltype((x + 5))>){
		//lvalue
	}
	
	if constexpr(!std::is_lvalue_reference_v<decltype((x + 5))>){
		//pr value
	}
	
	if constexpr(std::is_rvalue_reference_v<decltype((x + 5))>){
		//x value. T&& xvalue
	}

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------


class Student{

	void set_sddress(const std::string &address)
	{
		m_address = address;
	}
private:
	std::string m_address;
};



Bunu abartıp aşağıdaki gibi yazıyorlar

class Student{

	void set_sddress(std::string address)
	{
		m_address = std::move(address); // burada tekrar move edilmiş :) Abartıp böyle yazıyorlar 
	}
private:
	std::string m_address;
};

Bunun nasıl bir problemi var ????
Cevap : Bu fonksiyonu böyle yazmayın :):) Yukarıdaki gibi yazın :):)
Move kapasiteyi küçültüyor. Bu yüzden yapılmamalı. Aşağıda cevabı var.

--------------------------------------------------------------------------------------------------------------------------------------------------------------

AÇIKLAMAK IÇIN AŞAĞIDAKINI YAZDIK

int main()
{
	using namespace std;

	string sx(100'000, 'A');

	std::cout << "sx.size() = " << sx.size() << "\n";
	std::cout << "sx.capacity() = " << sx.capacity() << "\n";
	
	
	std::string sy(150,'Z');

	std::cout << "sy.size() = " << sy.size() << "\n";
	std::cout << "sy.capacity() = " << sy.capacity() << "\n";

	sx = sy; yaparsak burada çalışan = functionu sx in kapasitesine bakıyor, sy nin size ına bakıyor, reallocation gerekmiyorsa bunu yapmıyor.

	sx = std::move(sy); //Burada enteresan durum var. Move asisgnment kapasiteyi değiştirdi.Kapasiteyi küçülttü.
	//kapasite burada küçültülünce burada reallocation ihtiyacı doğuracak.Bir üsttekinde atama yapılınca kapasite yeterliyse realloc yapmıyor.

	std::cout << "sx.size() = " << sx.size() << "\n";
	std::cout << "sx.capacity() = " << sx.capacity() << "\n";
	
}

NOT !!!!

Kapasite reallocation ile otomatik büyür ama otomatik olarak shrink edilmez.
Eskiden vectorun kapasitesini shrink etmek için swap trick gibi bir  idiyom kullanılıyordu

vector<int>(ivec).swap(ivec); bu idiyom.
swapta kapasiteyi küçültüyor.

sx.erase(100); 100. index ve sonrasını tamamen siler
sx.erase(100,1); 100. cüyü siler
sx.erase(next(sx.begin(),100)); 100. cüyü siler
sx.erase(sx.begin() + 100); Buda 100. cüyü isler kontrol et.


NOT !!!!
Kapasiteyi küçülten functionlar 3 tane
swap, move, shrink_to_fit

--------------------------------------------------------------------------------------------------------------------------------------------------------------


NOEXCEPT VE TAŞIMA SEMANTİĞİ
----------------------------
Bu konu temel C++ ta ayrıltılı işlendi. Yinede önemli gördüklerimi yazdım.

Bir functionun noexcept ile nitelenmesi olması hata vermeyeceğini belirtir.

Eski C++ ta exception specification vardı. 
void func(int)throw(std::bad_alloc); böyle hangi türden hata vereceğini söylüyordu
çok kötü bir tasarım.

Kısaca noexcept tekrarı. Temel C++ notlarında detaylı var.
noexcept içinde compile time ifadesi yazıp onun noexcept verip vermediğini conditional olarak
sınayabiliriz. bu specifier

Birde noexcept operator var. Unevaluated context yaratıyor.

void func(int)noexcept(noexcept(foo())); böylede yazılabilir.

template <typename T>
void func(T x); // T türünün move ctoru excep. throw etmeme garantisi verirse bizde vereceğiz.


template <typename T>
void func(T x)noexcept(std::is_nothrow_move_constructible_v<T>);


NOEXCEPT TÜRÜN BIR PARÇASI AMA OVERLOADING AMAÇLI KULLANILAMIYOR !!!!!!!!!!!!!!

void func(int) noexcept;
void func(int);

int main()
{
	void (*fp)(int)noexcept = &func; //Bu hata.Noexcept garantisi olan ptr var ama func vermiyor.
									 // tam tersi geçerliydi. Temel C++ :D

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------


class Base{
public:
	virtual void foo(int);

	virtual void func(int)noexcept;
};

class Der : public base{
public:
	void foo(int)noexcept override; //Burada hata yok ama tersi olsaydı sentaks hatası olurdu

	void func(int)override; // Yukarıda noexcept ama burada yok.Sentaks hatası
};


MOLA

===============================================================================================================================================================
===============================================================================================================================================================
===============================================================================================================================================================
===============================================================================================================================================================

SINFILARIN ÖZEL ÜYE FONKSİYONLARI EĞER DERLEYİCİ TARAFINDAN DEFAULT EDİİYORSA BUNLARIN NOEXCEPT OLUP OLMADIĞI DERLEYİCİ TARAFINDAN BELİRLENİYOR
DESTRUCTOR DEFAULT OLARAK NOEXCEPT

class Myclass{

private:
};

Bu durumda tüm special member funclar derkeyici tarafından yazılır.
Sınıfın nonstatic veri elemanlarının, noexcept olup olmadığına bakacak, eğer noexcept ise bunlar kendi yazdığı functa noexcept olarak yazılır.

int main()
{
	using namespace std;

	cout << boolalpha;

	cout << is_nothrow_default_constructible_v<Myclass> << "\n";
	cout << is_nothrow_copy_constructible_v<Myclass> << "\n";
	cout << is_nothrow_move_constructible_v<Myclass> << "\n";
	cout << is_nothrow_copy_assignable_v<Myclass> << "\n";
	cout << is_nothrow_move_assignable_v<Myclass> << "\n"; // Bu durumda hepsi true. Çünkü noexcept operator ile baktı ve hepsi noexcept olarak yazılabilir
}								// zaten neye bakacak eleman yok :D:D:D mesela int x gibi birşey olsaydı primitive tür zaten bu
								// yine tüm special member funclar 

---------------------------------------------------------------------------------------------------------------------------------------------------------------

class Myclass{

private:
	int mx;
	std::string str;
};

int main()
{
	using namespace std;

	cout << boolalpha;

	cout << is_nothrow_default_constructible_v<Myclass> << "\n"; // true
	cout << is_nothrow_copy_constructible_v<Myclass> << "\n";   // false . string sınıfı için copy constructor nothrow değil çünkü.exception throw edebilir.
	cout << is_nothrow_move_constructible_v<Myclass> << "\n";   // true
	cout << is_nothrow_copy_assignable_v<Myclass> << "\n";		// false  string sınıfı için copy assign nothrow değil çünkü.exception throw edebilir.
	cout << is_nothrow_move_assignable_v<Myclass> << "\n";		// true
}															

Derleyici bu funcları yazdığında, bunları exception throw etmeme garantisini elemanlara bakarak compile time da belirliyor.


---------------------------------------------------------------------------------------------------------------------------------------------------------------

C++20 ILE BIR ÖZELLIK GELDI.

Burası C++20 değil

class Myclass{
public:
	Myclass(Myclass&&) = default;	 // bunu derleyiciye yazdırdık	
private:
	int mx;
	std::string str;
};

int main()
{
	using namespace std;

	cout << boolalpha;

	cout << is_nothrow_move_constructible_v<Myclass> << "\n"; // Yine true yazdı
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------


class Myclass{
public:
	Myclass(Myclass&&)noexcept = default; // C++20 ile geldi burası DİKKAT!!!!!!!!!!!!!!!!!!
	
private:
	int mx;
	std::string str;
};

int main()
{
	using namespace std;

	cout << boolalpha;

	cout << is_nothrow_move_constructible_v<Myclass> << "\n"; // Yine true yazdı. Dİkkat edersek noexcepti default ettirmişiz.
}


---------------------------------------------------------------------------------------------------------------------------------------------------------------

NEDEN MOVE CTORUN NOEXCEPT OLUP OLMAMASI ÖNEMLI
-----------------------------------------------
Eğer move ctor copy ctor gibi special member functionları kendimiz yazarsak bunların noexcept olup olmadığını kendimiz belirlemeliyiz.
Move ctorun exception throw etmeme garantisi vermesi çok önemli. Bazı functionların çok sıradışı durum yoksa noexcept olması gerekiyor.

Bu functionların default olarak noexcept olması gerektiğini düşünmeliyiz.
Bunlar, Move Ctor, Move Assignemnt, swap funcları ve allocation funcları, destructor ise zaten exception throw etmiyor.
Bunların exception throw etmeme garantisi vermesi gerekiyor. 

Eğer herhangibir şekilde öğrenmek istersek, static_assert ile test edebiliriz.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

Neden Move Ctorun noexcept olması önemli ?
Örnek üzerinde göreceğiz.

class Nec{
public:
	Nec() = default;

	Nec(const Nec&)
	{
		std::cout << "copy ctor\n";
	}

	Nec(Nec&&)
	{
		std::cout << "move ctor\n";
	}
};

int main()
{
	//Nec x,y,z;

	//std::vector<Nec> vec{x,y,z}; // copy ctorlar çağrıldı.YORUM YAPTIM ÇIKTIYI DAHA İYİ İNCELEMEK İÇİN

	std::vector<Nec> vec{Nec{}, Nec{}, Nec{}}; // copy ctorlar çağrıldı yine

	std::cout << "vec.size() = " << vec.size() << "\n";
	std::cout << "vec.capacity() = " << vec.capacity() << "\n";

	vec.push_back(Nec{}); //reallocation oldu 

}

ÇIKTI
-----
copy ctor
copy ctor
copy ctor
vec.size() = 3
vec.capacity() = 3
move ctor
copy ctor	// BURADA REALLOCATION OLDUĞUNDA COPY CTORLAR ÇAĞRILDI ÇÜNKÜ MOVE CTOR NOEXCEPT DEĞİL.
copy ctor
copy ctor


---------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE CTOR NOEXCEPT OLMASIN AMA YİNE TAŞIMA YAPILSIN İSTERSEK BUNUN NEDENİ NE?
Cevap : Exception Garantisi

basic - strong - nothrow
Hatırlatma yaptı
basicte memory leak yok, invalidation yok, state değişebilir
strong --> basic + state değişmez- commit or rollback
nothrow --> strong + hata vermeyecek. verirse kendi içinde handle edecek.

Temel C++ ta ayrıtılı işlendi.

Push_back strong garantisi vermek üzere tanımlandığı için burada eğer taşıma işlemi yapılsaydı bu garantiyi veremezdi.
Ama move da zaten taşınmış nesneyi hata durumunda eski haline döndürme şansı yok. Move ctorun noexcept garantisi vermemesi
sözkonusu ise derleyici kod seçiminde copy ctor seçiyor.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE CTORU NOEXCEPT YAPALIM

class Nec{
public:
	Nec() = default;

	Nec(const Nec&)
	{
		std::cout << "copy ctor\n";
	}

	Nec(Nec&&)noexcept
	{
		std::cout << "move ctor\n";
	}
};

int main()
{
	std::vector<Nec> vec{Nec{}, Nec{}, Nec{}}; // copy ctorlar çağrıldı yine

	std::cout << "vec.size() = " << vec.size() << "\n";
	std::cout << "vec.capacity() = " << vec.capacity() << "\n";

	vec.push_back(Nec{}); //reallocation oldu
}

ÇIKTI
-----
copy ctor
copy ctor
copy ctor
vec.size() = 3
vec.capacity() = 3
move ctor
move ctor	// DİKKAT. ARTIK KOPYALAMA YERINE TAŞIDI !!!!!!!!!!!!!!!!!
move ctor
move ctor


---------------------------------------------------------------------------------------------------------------------------------------------------------------

BU VERİM ARTIŞI SAĞLAR MI?

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class Nec {
public:
	Nec() : ms(1'000, 'A') {}

	Nec(const Nec& other) : ms{ other.ms } {}

	//Nec(Nec&&other) : ms{std::move(other.ms)} {}
	Nec(Nec&& other) noexcept : ms{ std::move(other.ms) } {} //noexcept olmayanıda çalıştır test et.

private:
	std::string ms;
};

int main()
{
	using namespace std::chrono;
	using namespace std;

	std::vector<Nec> vec(100'000);

	auto start = steady_clock::now();
	vec.resize(vec.capacity() + 1);
	auto end = steady_clock::now();

	cout << duration<double, milli>(end - start).count() << "ms\n";
}

ŞAKA gibi 200 kat falan farketti.Release modda test edildi.
Move Ctor noexcept edilince move ctor çapğrıldı ve böyle bir fark oluştu.Move Ctorun maliyeti copy ctora göre çok çok düşük.
3 adet pointer kopyalanıyor sadece.

---------------------------------------------------------------------------------------------------------------------------------------------------------------

class Nec{
public:
	Nec() = default;

	Nec(const Nec&)
	{
		std::cout << "copy ctor\n";
	}

	Nec(Nec&&)noexcept
	{
		std::cout << "move ctor\n";
	}
};

int main()
{
	using namespace std;

	vector<Nec>myvec(100);

	vector<Nec>xvec = myvec; // Burada 100 adet copy ctor

	vector<Nec> xvec = std::move(myvec); // çıktı yok.Çünkü vectorün 3 pointerı taşınacak.move işlemini Nec yapmıyor.
}


ARRAY için bakalım

int main()
{
	using namespace std;
	
	std::array<Nec,20>ax;
	std::array<Nec,20>ay = ax; // copy cotlar çağrıldı 20 adet
	
	std::array<Nec,20>ay = std::move(ax); // Burada Move ctorlar çağrıldı bu sefer.Bu vector değil.Array içinde bildiğimiz nesneler var herbirine move ctorlar ile
											// taşımalar yapılacak. İçinde pointerlar falan yok. :D:D:D . Linear complexity her eleman taşınacak. 
											// Vector gibi 3 poitner taşınmıyor. Zaten pointer yok. 
}

Array aşağıdaki gibi.
template <typename T, size_t n>
struct Array{
	T ma[n];
};


---------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE CTORU NOEXCPT MI DEĞIL MI TEST EDILIYOR

class Myclass{
public:
	Myclass();
};

class Nec{
	Myclass x;
};

int main()
{
	using namespace std;

	static_assert(is_nothrow_move_constructible_v<Myclass>, "Move Ctor noexcept degil");
}

C++17 den sonra 2. argüman olarak bir string literali vermek opsiyonel oldu.
ÖNcesinde değildi.

===============================================================================================================================================================
===============================================================================================================================================================
===============================================================================================================================================================
===============================================================================================================================================================


PERFECT FORWARDING
------------------

Benim doğrudan çağırabileceğim bir func var. Bu fonksiyonlar argüman veya argümanlar istiyor.
Ben bunu doğrudan çağırmak yerine bunu çağıracak bir fnc çağırmak istiyorum ve benim fonksiyonu çağırırken 
geçeceğim argümanları bu aracı functiona veriyorum.

Öyle bir iş yapılıyorki benim doğrudan çağırmamla bu fonksiyonun çağırması arasında bir fark olmasın

ESKİDEN NASIL ÇÖZÜLÜYORDU

class Nec{
public:
	
};

void func(Nec&)
{
	std::cout << "func(Nec&)\n";
}

void func(const Nec&)
{
	std::cout << "func(const Nec&)\n";
}

void func(Nec&&)
{
	std::cout << "func(Nec&&)\n";
}

void func(const Nec&&)
{
	std::cout << "const func(Nec&&)\n";
}

void call_func(Nec &x)
{
	func(x);		
}

void call_func(const Nec &x)
{
	func(x);		
}

void call_func(Nec &&x)
{
	func(std::move(x));		
}

void call_func(const Nec &&x)
{
	func(std::move(x));		
}

int main()
{
	Nec x;
	const Nec cx;

	func(x); call_func(x);
	std::cout << "------------------------------------\n"
	func(cx); call_func(cx);
	std::cout << "------------------------------------\n"

	func(Nec{}); call_func(Nec{});
	std::cout << "------------------------------------\n"
	
	func(std::move(x));call_func(std::move(x));
	std::cout << "------------------------------------\n"
	
	func(std::move(cx));call_func(std::move(cx));
	std::cout << "------------------------------------\n"
	
}

Benim çağırmamla call func arasında fark olmasın istiyoruz.
4 tane overload yaptık.

NASIL ÇÖZÜYORUZ ?
PERFECT FORWARDING İLE
---------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void call_func(T&&x, T&&y) // buradaki T&& sağ taraf referansı değil çünkü hem yalın halde hemde tür çıkarımı var.Forwarding referans veya Universal referans
{
	func(std::forward<T>(x));
}

Bu fonksiyon şablonu herhangibir argüman kabul eder. Hiçbir argüman için sentaks hatası olmayacak.Universal referense deme sebebi bu.
Mevlana esprisi :):) Kim olursan ol gel.const ol yada olma ne olursan ol gel :D:D:D:D

Forwarding referans ismi o kadar da iyi değil. Bu forward işlemi yapılacağı algısını yapıyor. Çoğunlukla yapacağız ama her zaman yapılacak diye birşey yok.

Buradaki kurallar baya karmaşık fakat karmaşıklığı yaratan burada kullanılan && decleratörü.
İnsanlar forwarding ref ile sağ taraf refi ayırd etmekte zorlanıyorlar.Hatta &&& kullanmayı düşünmüşler forwarding referans için.

Buradaki Template Argüment Deduction konusu Temel C++ ta çok detaylı gördük. Yazmayacağım.
T nin çıkarımıyla parametre çıkarımı farklı şeyler. BUNU OKUYAN HER KIM ISE, KESİNLİKLE BU KONUDAKİ NOTLARI OKUMALI.TEMEL C++


class Myclass{};

int main()
{
	Myclass m;
	func(Myclass{}); // T is Myclass
	func(m);		 // T ise Myclass&
}

Referansa referans bazı yapılarda oluşursa reference collapsing denen kurallar devreye giriyor.Yazmıyorum Temel C++
typedef isim bildirimlerinde veya explicit tür argümanı verirken karşımıza çıkabilir.







































*/
