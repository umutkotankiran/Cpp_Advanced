/*

BU DERS SOHBET HAVASINDA GEÇTİ BİRAZ

init list ctor ile fill ctor a girdik soru sorulmuş

string str(100,'a'); // 100 tane a tutar
string str{100,'a'};  // d ve a yı tutar. d nin Asci deki yeri 100

vector<int>vec(100); // size 100
vector<int>vec{100}; // size 1
vector<int>vec{100,45}; // size 2. tutulan öğeler 100 ve 45

-------------------------------------------------------------------------------------------------------------------------

class Myclass{
public:
	Myclass() = default;
	Myclass(const Myclass&)
	{
		std::cout<< "copy ctor\n";
	}
	Myclass(Myclass&&)
	{
		std::cout<< "move ctor\n";
	}
};

void func(std::initializer_list<Myclass> y)
{
	//.. 
}

int main()
{
	using namespace std;

	initializer_list<Myclass> x{Myclass{},Myclass{},Myclass{}}; //Default Ctor çağrılıyor.Mandatory copy ellsion(simplified value categories)

	-----------------------------------------------------------------------------------------------------------------

	Myclass m1,m2,m3; // isimlendirdik ve copy ellision imkanıda kalmadı.

	initializer_list<Myclass> x{m1,m2,m3}; //copy ctor 3 kere çağrılıyor.

	-----------------------------------------------------------------------------------------------------------------

	func(x); // Copy Ctor çağrılmaz çünkü referans semantiği ile geçiliyor init list.
			 // Her ne kadar func parametresi referans olmasada call by value görünsede init list her zaman
			 // referans semantiği ile aktarılıyor. Ayrıca İnit list öğeleri const ele alınır. Değiştirilmeye açık değil.
			 // argüman olan x ile parametre olan y farklı nesneler değil.

	x ile y nin adreslerini yazdırınca farklı nesneler çıkıyor ama bu burada bir kopyalama olmadığı gerçeğini değiştirmez.

}

-----------------------------------------------------------------------------------------------------------------

INIT LISTTE EĞER SINIFIN COPY CTORU YOKSA YANI MOVE ONLY TYPE BIR TÜRÜMÜZ VARSA BU DURUMDA NORMALDE INIT LISTTE
KOPYALAMA SEMANTIĞI OLMADIĞINDAN, INIT LISTTE ÖĞELERI LISTE IÇINDE ISIMLENDIRILMIŞ NESNELERLE OLUŞTURAMAYIZ.

class Myclass{
public:
	Myclass() = default;
	Myclass(const Myclass&) = delete; // zaten delete edilecek ama açıkça yazdık.
	
	Myclass(Myclass&&)
	{
		std::cout<< "move ctor\n";
	}
};

int main()
{
	using namespace std;
	Myclass x,y,z;

	initializer_list<Myclass>m{x,y,z}; // Bu durumda Kopyalama yapılması gerekiyor.
}										// Kopyalamaya kapalı olduğundan sentaks hatası.


-----------------------------------------------------------------------------------------------------------------

SCOTT MEYERS IN DILDE HATA OLDUĞUNU SÖYLEDIĞI KISIM

Template deduction ile auto deduction aynı. Initializer list hariç.
CTAD : Ctor yoluyla template argümanı çıkarımı yapılmasına deniyordu

template <typename T>
void func(T x);

int main()
{
	func(12);

	auto x = 12; // tür çıkarımı aynı.
	// tek farklılık initializer list çıkarımında

	auto x = {1,2,3,4,5}; // Burada çıkarım yapılyıor ama templatelerde farklı.initializer_list<int>

	func({1,2,3,4,5}); //SENTAKS HATASI
}

------------------------------------------------------------------------------------------------------------

Dilin kuralları burada 2 kere değiştirilmiş

int main()
{
	auto x = {1,3,5}; // init_list<3> açılımı türü
	auto x{1}; // burası artık int. Eskiden init list<int> idi bu.
	auto x{1,3,4}; // SENTAKS HATASI.Eskiden legaldi
}

------------------------------------------------------------------------------------------------------------

İnitializer list gibi davranan sınıf nasıl yazılır

#include <functional>

int main()
{
	using namespace std;
	int x = 190;

	auto y = ref(x); // y nin türü reference wrapper ın int açılımı
}


Yazalım Şimdi

void func(std::reference_wrapper<int> x)
{
	x.get() = 45;
}

int main()
{
	using namespace std;
	int x = 190;

	auto y = ref(x); // y nin türü reference wrapper ın int açılımı
	func(y);

	std::cout << "y = " << y << "\n";
}

--------------------------------------------------------------

Reference Wrapper ın Önemi

template <typename T>
void func(T x)
{
	x = 98;
}

int main()
{
	int ival{245};
	func(ival);  // burada değişmez
	func(ref(ival));  // burada değişir. hata veriyor çünkü get() etmedik.Buna bakacağım
					  // Reference wrapperın bir tür dönüştürme operatörü var. T& a dönüşüm yapıyor.
					  // Burada call by value olacak func yapısını call by reference haline getirdik.	  
}

Bundan fayda sağlanılan yerler var. yani call by value olarak kullanılması yerine reference wrapper ile kullanılması
fayda sağlanan yerler var.

Ör:

class pred{
public:
	bool operator()(int)const
};

int main()
{
	vector<int> ivec;

	Pred p;

	auto iter = find_if(ivec.begin(), ivec.end(), p);  // Bunu Bu şekilde yazmak yerine aşağıdaki gibi yazılırsa
														// kazanç olur
}

--------------------------------------------------------------

find_if hatırlatması

template <typename InIter, typename Pred>
InIter Find_if(InIter beg, InIter end, Pred f)
{
	while(beg != end)
	{
		if(f(*beg))
			return beg;
		++beg;
	}
	return beg;
}


class BigPred{
public:
	bool operator()(int)const;
};

int main()
{
	BigPred p;
	vector<int> ivec;
	auto iter = find_if(ivec.begin(), ivec.end(),ref(p)); // DİKKAT!!! ref(p)
}

// Burada Predicate function sizeofça çok büyük olabilirdi yani ciddi kopyalama maliyeti var.
// bu yüzden referans semantiği ile geçtik bunu.

----------------------------------------------------------------------------------------------------------------------

BURADAN SONRA ARTIK TAŞIMA SEMANTİĞİNE DÖNÜYORUZ

void func(Myclass x)
{
	std::vector<Myclass> vec;

	vec.push_back(x); // tüm containerların member funcları overload edilmiş.

	vec.push_back(std::move(x)); // şimdi farklı overload çalıştı.
}

----------------------------------------------------------------------------------------------------------------------

COPY ELLISION VE MOVE SEMANTICS İLE İLGİLİ ÖNERİLER

1.
void func(vector<int> &vec); eskiden böyleydi 
 // return değerine bak.
 // Yerine out parametre koyup yani biz ona vector verelim o vectore yazsın deniyordu

vector<int> func(); // Bu eskiden günahtı :D Artık böyle yazılabilir!!!!
Burada artık return value optimization olma ihtimali var.

2. 
Derleyiciyi yönlendirmeye çalışmayın.Derleyici bizden akıllı :)
mesela
return std::move(x); yazmak mandatory copy ellisionu engelleyebiliyor.

3. 
Sınıfın move ctoru eğer kopyalamaya göre avantaj ise bunun yazılması lazım.

4.
Functionları kısa tutmak.Uzun functionlar 15-20 yi geçmemeli.
Uzun kodlar copy ellisionu da zorlaştırıyor.

5.
Programcıların bir kısmı compile time programlamasını kullanmıyor olmaları.
Mesela 10. Asal sayı baştan belli. Bunu runtime da hesaplamak saçma.
Compile time da hesaplamak mantıklı.

MOLA

====================================================================================================================
====================================================================================================================

1. soru?
Bir takım lideri demişki, range base for loop kullanırken sağ taraf değeri container kullanmayın demiş.

NOT : BU KULLANIMDA SORUN YOK AMA SORUN OLANLARI GÖRECEĞİZ !!!!

std::vector<std::string> foo();

int main()
{
	for(auto x : foo()) // team lead bunu kullanmayın demiş.
	{
		x; 
	}
}

Bunu söylerken makul bir ilke mi bu?
Team Lead in bu ilkeyi koyma sebebi şuysa, Programcı C++ a yeterince hakim değil.

2. Soru
Peki böyle bir ilke makul olmamasına rağmen ama team lead konuya hakimse, neden böyle bir ilke koyuyor?
Tİpik bir örnek. Konuya hakim olmayan programcılar bazı araçları kullaınrken hata yapabiliyorlar.
Böyle bir ilke olmamalı hata yok burada, ama dikkatli kullanmazsak range for base for loop yazayım derken
başımızı ciddi derde sokabiliriz.UB durumuna düşebiliriz.

Team Lead hangi senaryodan korkuyordur?
DANGLING REFERANS. Çok sık yapılıyor.

Birazdan bu konuyla ilgili örnekler verilecek
Buradaki sorunun içerdiği başka bir soru range base for loop için compilerın nasıl bir kod ürettiği.
Programcı range base for loop için nasıl kod üretildiğini programcı bilse bir problem kalmayacak.
Ya kullanmayacak ya da doğru şekilde kullanacak range base for loopu.

---------------------------------------------------------------------------------------------------------------------

EN SON TAŞIMA SEMANTİĞİNDEN NASIL FAYDALANIRIZ DİYORDUK.

Geçen ders konuştuğumuz func parametreleri tür tür& ve tür && olma durumlarında move copy veya man copy ellision durumları
anlatıldı.

void func(const std::string &str) //yaptığı iş gereği burada bir başka string kullanması gerekiyor ama bu string reverse edilmiş hali olacak.
{
	std::string x{ str };

	reverse(begin(x), end(x)); // Bu legal ADL
	
}

Bu kodda rahatsızlık veren bir durum var mı? EVET
Burada parametre sol taraf referansı olması yerine doğrudan call by value yapmak.

void func(std::string str) 
{
	reverse(begin(x), end(x)); // Bu legal ADL
}

Burada kazancımız bir kopyalama vardı az önce ama kopyalama func fonksiyonunun kodu içindeydi ama şimdi kopyalama
argümandan parametre değişkenine göndermede yapılacak(taşıma semantiğide olabilir tabi).

Ama diğerine göre avantajı şimdi artık burada bir pr value expr ile ilk değer verildiğinde o zaman mandatory copy ellision
yapma şansımda var.DOlayısı ile parametreleri sınıf türünden yapma ciddi avantaj olabilir. Her zaman yapılır denemez ama
bazı durumlarda özellikle parametreyi referans yapıp fonksiyonun kodu içinde onun tekrar kopyasını çıkartıyorsak doğrudan parametreyi
sınıf türünden yapmak daha iyi seçenek olabilir.

----------------------------------------------------------------------------------------------------------------------

İsimlendirilmiş nesne kullanmaktan mümkün olduğunca kaçınmak gerekiyor. birçok kodda programcı bir rvalue expr bir geçici
nesne oluşturmak yerine nesneye isim veriyor. Nesneye isim verince başka problemlerin kapısını açıyor.

Özellike func a gönderilen argümanlarda.

string x{********};
foo(x); // Sadece bu funca arügman olarak gönderilmek üzere x oluşturulduysa bu kodu böyle yazmak yerine

foo(string{*********}); // Bu kodu tercih etmek için argümanları sayalım


Geçici Nesnenin Avantajları
---------------------------
1. Mandatory Copy Ellision
2. scope leakage(C den gelenler bunu uygularken problem yaşıyor.)
3. String sınıfı tipik bir RAII sınıfı.Yani bir kaynağı tutuyor. Destructoru çağrılmadığı için halen bu kaynağı tutuyor.
4. Bir ismi yazarken yanlışlıkla onun yerine başka bir ismi yazmak.a yerine b yazmak gibi. Mesela Ortada bir isim var 
   Bu isim visible olmaması gereken yerde visible hale geliyor. Bu hale gelince başka isim yerine yanlışlıkla bu ismi
   yazma biçimindeki kodlama hatası riski artıyor.


Programcılar şunu yapabiliyor bazen.Nesnenin scopu nu bitiriyor.

int main()
{
	{
		string x{********};
		foo(x); 
	}// Burada x in destructoru çağrılacak scope u bittiği için
}


Geçici Nesnenin Dezavantajları
------------------------------
1. Kodun okumasını geçici nesneler zorlaştırabilir. Bunun için zaten isimler kullanılıyor dersekte aşağıdaki yapılabilir.
Bir ismin kullanılmayacağı algısını kuvvetlendirmek için hemde copy yerine move dan faydalanmak için bunu functiona geçerken
move edebiliriz.

int main()
{
	string s{};

	func(std::move(s)); // Artık bir daha kullanılma niteliği taşımadığını anlatıyoruz.Taşımadan da faydalanıyoruz.
}


2. Nesneyi birden fazla kez kullanmamız gerektiği durumlarda avantajsız olur çünkü nesneyi birden çok kullanmak gerekirse
ne kadar kullanırsak o kadar ctor çağrılır. Nesneyi tekrar kullanmak söz konusuysa o zaman 2 kere taşıma olamayacağına göre,
ilk kullandığımızı kopyalama son kullanılanı ise taşımaya çevirebiliriz.

Myclass mx;

vec.push_back(mx); // Copy Ctor
vec.push_back(std::move(mx)); // Move Ctor. artık kullanılmayacak çünkü


---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------

KALDIĞIMIZ YERDEN DEVAM. TAŞIMA SEMANTİĞİNDEN FAYDALANAN DİĞER DURUMLAR

En çok tartışılan konulardan biri. Bir sınıf var ele elemanları var. Bunları copy etmek yüksek maliyette.

class Person{
public:
	std::string get_address()const // BUNUN DEZAVANTAJI NE OLABİLİR ? MALİYET 
	{
		return m_address;
	}

	std::vector<std::string> get_friends()const;
private:
	std::string m_address; // bu string çok yüksek boyutlu gibi düşünelim
	std::vector<std::string> m_friends;
};

int main()
{
	Person x;

	x.get_address();	// bazı kullanım senaryolarında bu çok tehlikeli oluyor.

	if(x.get_friends().empty()) // Burada sadece boş olup olmadığına baktık ama buna bakmak, x.get_friends() ile m_friend
	{							// kopyasını çıkarttığımız gerçeğini değiştirmiyor.  
			
	}
}

-------------------------------------------------------------------------

Burada referans döndürebiliriz.

class Person{
public:
	std::string& get_address()const
	{
		return m_address;
	}

	const std::vector<std::string> &get_friends()const;
private:
	std::string m_address;
	std::vector<std::string> m_friends;	
};

int main()
{
	Person per;

	if(per.get_friends().empty()) // Burada referans dönüldü maliyeti çok az.Ama Dangling Referans olma riski var.
	{
	}
}

Dangling referans ın nereden geleceği belli olmaz.

----------------------------------------------------------------------------------------------------------------------

BU DANGLING REFERANS NASIL OLUŞABILIR

Dersin başında team lead range base for loopta rvalue expr kullanma demişti.
Bunu Söyleme sebebi, dangling referans. Team lead olarak bu riski almak istemiyor çünkü farkedilmeyebilir.

Dangling Referans oluşturalım


struct Person{
	std::string& get_name()const
	{
		return m_name;
	}
	
	std::string m_name{"Ali Kalan"};
};

Person create_person()
{
	Person p;

	return p;
}

int main()
{
	for(auto c : create_person().get_name())
	{
		std::cout.put(c); // UNDEFINED BEHAVIOR VAR. DANGLING REFERENCE VAR		
	}
}

BURADAKİ DANGLİNG REFERENCE IN SEBEBİ LIFE EXTENSION. GET_NAME İN RETURN DEĞERİ REFERENCE OLMASAYDI 
KESİNLİKLE DANGLİNG REFERENCE OLMAYACAKTI. CONST VE & SİLİNİP TEST EDİLEBİLİR.
BURADA KULLANDIĞIMIZ GET_NAME, CREATE_PERSONDA RETURN EDİLEN NESNENİN GET_NAME İ.RETURN EDİLEN NESNENİN
HAYATI BİTİYOR. YANİ CREATE_PERSON ÇAĞRILDIĞINDA FUNCTİONUN RETURN DEĞERİ OLAN NESNENİN HAYATI BİTİYOR.
BİZ HAYATI BİTMİŞ BİR FONKSİYONUN GET_NAME FUNCTİONUNU ÇAĞIRIYORUZ.

BURADA RANGE BASE FOR LOOP İÇİN DERLEYİCİ NASIL BİR KOD ÜRETİYOR BİLMEK LAZIM.
BURADA NASIL BİR DANGLİNG REFERENCE VAR BU ELEŞTİRİLERE KONU. BURADA VERİLEN YENİ BİR RANGE BASE FOR LOOP
ÖNERİSİ VAR. ONLAR KABUL EDİLSEYDİ BURADA DANGLİNG REFERENCE OLMAYACAKTI.

auto &&r = range;	// Buradaki range, : nin sağındaki range. r burada forwarding reference 
					// range lvalue expr ise r lvalue ref. r ise rvalue ref olacak

Derleyici aşağıdakini oluşturuyor

for(auto beg = r.begin(), auto end = r.end(); beg != end; ++beg)
{
	auto c = *beg;
}

Bizim kodumu aşağıdakine dönüştü yani.

auto &&r = create_person().get_name();
//BURAYA GELDIĞIMIZDE R DANGLING DURUMDA.HAYATI BİTMİŞ FUNCTIONUN FUNC INA ÇAĞRI İFADESİNİ TUTAN REFERENCE
CREATE_PERSON() DAN HEMEN SONRA . YA GELDIĞIMIZ ANDA HAYATI BIRMIŞ BIR FUNCTION OLUYOR.

for(auto beg = r.begin(), auto end = r.end(); beg != end; ++beg)
{
	auto c = *beg;
}


-----------------------------------------------------------------------------------------------------------------

KONUYLA İLGİLİ 2 ÖRNEK

class A {
public:
  ~A()
  {
	std::cout << "A destructor\n";
  }
};

class B {
public:
  ~B()
  {
	std::cout << "B destructor\n";
  }
  A ax;
  const A &geta()const		// DİKKAT !!!!!!!!!!!!!!
  {
	return ax;
  }
};


B get_b()
{
  B bx;

  return bx;
}


int main()
{
  const auto& r = get_b().geta();
  getchar();
}

Bu örnekte referans var. Getchar dan sonra çağrılan bir destructor yok !!!!!!!
Yani referns tamamen boşa çıkmış.Eğer referans dangling olmasaydı referansa bağlanan nesne r nin kapsamı bittiğinde
destroy olurdu !!!!!!!!!!!!!!

ÇIKTI
-----
B destructor
A destructor

getchar() çağrısı

-------------------------------------

class A {
public:
  ~A()
  {
	std::cout << "A destructor\n";
  }
};

class B {
public:
  ~B()
  {
	std::cout << "B destructor\n";
  }
  A ax;
  A geta()const		// DİKKAT !!!!!!!!!!!!!!
  {
	return ax;
  }
};


B get_b()
{
  B bx;

  return bx;
}


int main()
{
  const auto& r = get_b().geta();
  getchar();
}

Output
------
B destructor
A destructor

A destructor


BU İKİ ÖRNEĞİ HOCAYA SORDUM. ÇOK AÇIKLAYICI.
BUrada ise getchar dan sonra bir nesne daha destructor çağrılacak yanni reference scopeu bittiğinde bir 
destructor daha çağrılacak. Demekki referans dangling değil.Life extension olmuş 


==================================================================================================================
==================================================================================================================
==================================================================================================================
==================================================================================================================
==================================================================================================================


REFERENCE QUALIFIERS
--------------------
Modern C++ 11 ile geldi. 
Overloading i value kategorisi ile belirleyebiliyoruz bu sayede.
Mesela bir member func ı sadece rvalue için çağrılmasını sağlayabiliriz. Reference Qualifier ile sağlıyoruz bunu.


class Myclass{
public:
	void func();   
	void func()const;   // [] back front ... böyle yazılmış.const için bir overload const olmayan için diğer overload.

};


ör:
int main()
{
	Myclass m;

	Myclass{} = m; // burada birşey elde edemeyiz ama kod legal.
}

----------------------------------------------------------------------------------------------------------------

class Myclass{
public:
	void func()const &	// Sadece const Lvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "const func()&\n";
	}
	
	void func()& // Sadece Lvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "func()&\n";
	}
	
	void func()&& // Sadece Rvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "func()&&\n";
	}
	
	void func()const && // Sadece Rvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "const func()&&\n";
	}
};

int main()
{
	Myclass m;
	const Myclass cm;

	m.func();
	Myclass{}.func();
	std::move(m).func();

	cm.func();
	std::move(cm).func();
}


Burada Lvalue, Rvalue, const Lvalue, const Rvalue hertürlü ihtimali yazabiliriz.
Dangling referans sorunuda böyle çözülebilir. Birçok yerde kullanılıyor.

Copy assignment ı sağ taraf için engelleyebiliriz. Herşeyi engelleyebiliriz veya izin verebiliriz.

class Myclass
{
public:
	Myclass& operator=(const Myclass&); // derleyici bunu yazar implicitly declared durumda

	Myclass& operator=(const Myclass&)&; // böyle yazabiliriz artık.Sadece lvalue expr için çağrılır.

	Myclass& operator=(const Myclass&)& = default; // Böyle user declared defaulted ettirebiliriz.

};

*/
