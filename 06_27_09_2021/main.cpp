/*

Forwarding Referans tan Proxy class tan bahsettik en son

vector<bool> açılımı olayı.Bir önceki derste çok detaylı bakıldı en son örneğe

------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------

auto &&r = 10; // Forwarding Reference.Universal Reference.Çılgın Scott Meyers

------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------

C++ ta aşağıdaki çok sık kullanılıyor. Bir func return değeri diğer func a argüman olarak gönderiliyor.

func(foo())

foo nun return değeri lvalue reference olabilir rvalue reference olabilir. const lvalue ref veya value olabilir.

------------------------------------------------------------------------------------------------------------------------------------------------------------

YUKARIDAKİ SENARYOYU OLUŞTURMAYA ÇALIŞALIM.


class Nec{
};

Nec retval()
{
	Nec x;
	return x;
}

Nec& retlref(Nec &r)
{
	return r;
}

const Nec& retconstlref(const Nec &r)
{
	return r;
}

Nec&& retrref(Nec x)
{
	return std::move(x);
}

void foo(Nec &)
{
	std::cout << "foo(Nec&)\n";
}

void foo(const Nec &)
{
	std::cout << "const foo(Nec&)\n";
}

void foo(Nec &&)
{
	std::cout << "foo(Nec&&)\n";
}

int main()
{
	Nec x;

	foo(retlref(x)); // foo(Nec&) çalıştı

	foo(retconstlref(x)); // foo(const Nec&) çalıştı

	foo(retval(x)); // foo(Nec&&) çalıştı

	Aslında bir func a yapılan çağrıyı doğrudan diğerine argüman olarak geçersek value categori korunuyor.

	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	ŞÖYLE BIR PROBLEM VAR. 
	RETURN DEĞERINI DEĞIŞKENDE TUTMAK ISTIYORUM, ONDAN SONRA ARADA BAŞKA KODLAR OLABILIR, SONRADA FOO YU ÇAĞIRACAĞIZ VE YINEDE VALUE CATEGORI VE CONSTLUK KORUNACAK

	Nec x;

	auto retval = retconstlref(x);

	foo(retval); // teplate kodu olduğunda return değeri compile timeda template argümanına bağlı dğeişecek

	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	BURADA NE YAPMAMIZ LAZIM KI BU DEĞIŞKEN BU DEĞERI TUTTUKTAN SONRA TEKRAR FUNC A GÖNDERDIĞINDE FUNC I TEKRAR RETURN DEĞERI ILE ÇAĞIRMAKLA RETURN DEĞERINI TUTAN
	DEĞIŞKENLE ÇAĞIRMAK ARASINDA FARK KALMASIN. UNIVERSAL REFERENCE

	auto&&r = retconstlref(x);

	foo(std::forward<decltype(r)>(r)); // Düzgün çalıştı. calls foo(const Nec&)
	
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
	BAŞKA BIRIYLE DENEYELIM

	Nec x;

	auto&&r = retval(x);  //forwarding reference

	foo(std::forward<decltype(r)>(r)); //Yine düzgün çalıştı. calls foo(Nec&&)

}

TEMPLATE KODLARDA IHTIYAÇ OLUYOR BUNA

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BURADA DA BAŞKA BİR MESELE VAR :D:D:D

template <typename Func,typename ...Args>
?? call(Func f, Args&&... args)					// Bu functionun return değeri lvalue ref ise 
{								// bizim funcında lvalue ref olacak. Rvalue ise rvalue olacak
								// doğrudan değer ise değer olacak

	f(std::forward<Args>(args)...);				// Bu durumda return değeri burada çağrılan funcın return değeri olsa fakat return değerini f in forward edeceğiz				 
}								// Yani f in return değeri lvalue reference ise bizim call func ın return değeride lvalue ref olacak.
								// f, rvalue ref return ederse call da rvalue ref dönecek, f eğer value dönerse call da value dönecek...


BURADA NE ŞEKILDE FUNC ÇAĞRISINDAN ELDE EDILEN RETURN DEĞERINI VALUE CATEGORY VE CONSLUK KAYBEDILMEDEN CALL FUNCTIONU KENDISINI ÇAĞIRANA DOĞRU ŞEKILDE ILETECEK?
DECLTYPE AUTO ILE HALLEDILECEK. BU BIR PLACEHOLDER

Decltype(auto) nedir ?
auto olmaz, neden ? & lık ve constlukları almayabiliyor
auto& olmaz, neden ? bu sol taraf referansı, rvalue expr gelirse sentaks hatası
auto&& olmaz, neden ? Forwarding reference kurallarına göre return değeri ya sol ya sağ taraf referansı olur. Mesela bir string türü olma ihtimali yok.referans olmayan tür olamıyor :D:D

AŞAĞIDAKİ DURUMLARI İNCELEYİP SORUYA GERİ DÖNECEĞİZ!!!!

int foo()
{
	int x{10};
	return x;   // UB YOK
}

int main()
{
	const int&r = foo(); // Life extension var. UB YOK
}

-------------------------------------------------------

BU SORU ÇOK KRİTİK !!!!!

const int& func()
{
	return foo(); // UNDEFINED BEAVIOR. FOO NUN ÖMRÜ SCOPE SONUNDA BİTİYOR ÇÜNKÜ 
}

int main()
{
	const int&r = func();
}

-------------------------------------------------------

KISCA DECLTYPE(AUTO)
auto + decltype gibi çalışıyor.

int&& foo();

int main()
{
	int x = 5;
	decltype(auto) y = x; // int
	decltype(auto) y = 5; // int
	decltype(auto) z = foo(); // int&&
	decltype(auto) z = (x); // int&

	-------------------------------------------------------

	Decltype ta referanslık ve constluk düşmez
	const int x = 5;
	decltype(auto) y = x; // const int

	-------------------------------------------------------

	int x = 23;
	int &ref = x;
	decltype(auto) y = ref; // int&

	-------------------------------------------------------

	int x = 23;
	int&&ref = 10;
	decltype(auto) y = ref; // SENTAKS HATASI.sol taraf değerini, sağ taraf referansına bağlamaya çalıştık. ref isim formunda sol taraf değeri

}

decltype(auto) func()
{
	return 19; //return değeri int oldu
} 

-------------------------------------------------------

int&& foo();
decltype(auto) func()
{
	return foo(); // int&& return değeri
}

-------------------------------------------------------

int x = 10;
decltype(auto) func()
{
	return x; //int
	return (x); // int& olurdu
}

EĞER ÇAĞIRDIĞIMIZ FUNC IN RETURN DEĞERİNİ PERFECT FORWARD ETMEK İSTERSEK TEK YAPAMIZ GEREKEN FUNC IN RETURN DEĞERİ İFADESİNİ DECLTYPE(AUTO) YAPACAĞIZ

template <typename T>
decltype(auto) func(T x)
{
	return foo(x); // Hertürlü value category constluk korunacak
}



---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <utility>

class Myclass{};
Myclass foo();
Myclass& bar();
Myclass&& baz();

int main()
{
	Myclass mx;
	const Myclass cmx;
	Myclass& r = mx;
	const Myclass& cr = mx;

	decltype(auto) m1 = mx;					//Myclass m1 = mx;
	decltype(auto) m2 = cmx;				//const Myclass m2 = cmx;
	decltype(auto) m3 = r;					//Myclass& m3 = r;
	decltype(auto) m4 = cr;					//const Myclass& m4 = cr;
	decltype(auto) m5 = foo();				//Myclass m5 = foo();
	decltype(auto) m6 = bar();				//Myclass& m6 = bar();
	decltype(auto) m7 = baz();				//Myclass&& m7 = baz();
	decltype(auto) m8 = (mx);				//Myclass& m8 = mx;
	decltype(auto) m9 = (cmx);				//const Myclass& m9 = cmx;
	decltype(auto) m10 = (r);				//Myclass& m10 = r;
	decltype(auto) m11 = (cr);				//const Myclass& m11 = cr;
	decltype(auto) m12 = std::move(mx);		//Myclass&& m12 = std::move(mx);
	decltype(auto) m13 = std::move(cmx);	//const Myclass&& m13 = std::move(cmx);
	decltype(auto) m14 = Myclass{};			//Myclass m14 = Myclass{}
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SORUYA GERİ DÖNELİM TEKRAR YAZAYIM TEMPLATEİ

template <typename Func,typename ...Args>
decltype(auto) call(Func f, Args&&... args)		
{												
	f(std::forward<Args>(args)...);	// Burada sorun yok şuanda

	--------------------------------------------------------------

	BÖYLE OLSAYDI
	decltype(auto) retval = f(std::forward<Args>(args)...); // RETVAL CONSTLUK VE VALUE CATEGORY YI KORUDU AMA RETVALI NASIL DÖNDÜRECEĞIZ ????

	if constexpr (std::is_rvalue_reference_v<decltype(retval)>) // retval rvalue reference ise
	{
		return std::move(retval); // if e girerse burada retval rvalue reference. func return type decltype(auto). move a çağrıda x value expr olduğuna göre.Burada bu func ın return
									// değeri yani call ın return değeri yine rvalue reference olacak.
	}
	else
	{
		return retval; //Ya value döndürür yada lvalue ref kaldı geriye.
	}
}												

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LAMBDA IFADESI OLDUĞUNDA NE YAPACAĞIZ

int main()
{
	auto f = [](auto func, auto&& ...args) ->decltype(auto) {
		func(std::forward<decltype(args)>(args)...)
	};
}

MOLA

=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================

MOLADAN ÖNCEKİ UNDEFINED BEHAVIOR SORUSU AÇIKLANDI 1:04

class Myclass{};

Myclass& func()
{
	Myclass x;

	return x; // TABİKİ TANIMSIZ DAVRANIŞ
}

C deki karşılığı
Myclass* func()
{
	Myclass x;

	return &x; // TANIMSIZ DAVRANIŞ
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE_IF_NOEXCEPT
----------------
Bu function argüman olarak kullanılan sınıf nesnesi onun move ctoru noexcept ise move işlemi yapıyor ama noeexcept değilse return değeri sol taraf değeri oluyor.


class Nec{}
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
	Nec x;

	Nec y = x; // burada copy ctor çağrılır.

	Nec y = std::move(x); // burada move ctor çağrılır

	Nec y = std::move_if_noexcept(x);  // Burada Move Ctor noexcept ise move ctor çağrılır, ama x in ait olduğu sınıfın move ctoru noexcept değilse copy ctor çağrılır. Nec y = x; gibi aynen
}


=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================

MOVE ONLY TYPES
---------------
Standart library nin taşıma semantiği ve perfect forwarding ile ilgili verdiği destek ile girmiştik bu konuya


CONTAINERLAR
move members ve copy members ları var.Move ctor diğer nesnenin kaynağını çalıyor. Eğer bir container başka bir containera taşınırsa moved from state teki obje kaynağını
kaybetmiş olacak, ve bu sabit karmaşıklıta olacak çünkü elemanlar değil pointerlar kopyalanıyor. Mesela standartlar taşınmış bir vector(moved from stateteki) boş olmak zorunda diyor.


Diğer containerlarda taşımış durumda olan containerlar boş olma garantisinde değiller.Ama genelde boş oluyorlar.

Moved From Statete nesne kullanılabilir durumda ama değerinin ne olduğu konusunda öngörüde bulunamıyoruz.

Containerlar sözkonusu olunca vectorde zaten boş olması garanti altında, diğerlerinde de implementasyona bağlı.
Neden vector boş olmalı ? 
- Noexcept garantisi veriyorsa zaten diğerini belirli bir değerde bırakma ihtimali kalmıyor.
- Move Ctorun constant time olma garantisi var. Constant time da olupda diğerinin boş kalmaması gibi garanti yok. Yani eleman eleman taşıma yapmıyor.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE ASSIGNMENTTA DURUM BÖYLE DEĞIL

Mesela move assignmentta nasıl bir kod olabilir.
x = y;  // burada 2 seçenek var gibi duruyor.

Burada önce x in nesneleri destroy edilip mi taşıma yapılıyor yoksa eleman eleman taşıma yapılıyorda kalan nesneler mi destroy ediliyor ???????
Move Asssignment standart allocatorlar varsa önce x in elemanları destroy edilip sonra y den x e taşıma yapılıyor şeklinde düşünebiliriz.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CONTAINERLARIN INSERT FUNCLARI 
Array hariç.Array bildiğimiz C dizilerinin sarmalayıcısı. 

İnsert funcları pushback pushfront funcları overload edilmiş, lvalue reference, const lvalue reference ve rvalue reference yapılmış.
- Dolayısı ile bir containera insert func a argüman olarak rvalue expr geçtiğimizde zaten onun taşınması sonucunu doğuruyoruz
- Containerların içinde emplace ismi geçen funcları perfect forwarding yapıyor aynı zamanda duruma göre gönderdiğimiz argümanın ne olduğuna bağlı olarak taşıma işlemi demek
  Gönderdiğimiz argümanlardan biri rvalue expr ise taşımayı tetikleyebilecek.

A - Taşıma semantiği nedeniyle func ın return değerini container yapabiliriz.
B - Containera sağ taraf değeri ifadeler ile eleman eklersek taşıma semantğinden faydalanabiliriz.

Bazı idiyomlarda move semantics ten faydalanılıyor
ör:
while(ifs >> str)
	vec.push_back(std::move(str));


Iostream sınıflar içinde taşıma semantiği geçerli.
Bunu move only type ele alındıktan sonra işleyeceğiz :D

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MOVE ONLY TYPE 
Kopyalamaya kapalı ama taşımaya açık sınıflar.
unique_ptr, iostream, thread... gibi sınıflar move only types. Bunlarda kopyalamanın anlamı olmadığı için kopyalama kapatılmış.

class MoveOnly {
public:
	MoveOnly() = default;
	//...
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly& operator=(const MoveOnly&) = delete;
	MoveOnly(MoveOnly&&);
	MoveOnly& operator=(MoveOnly&&);
};


Aslında delete etmesekte halen move only çünkü move memberlar tanımlanınca copy memberlar delete ediliyor.Ama yukarıdaki daha yaygın

class MoveOnly2 {
public:
	MoveOnly2() = default;
	//...
	MoveOnly2(MoveOnly2&&);
	MoveOnly2& operator=(MoveOnly2&&);
};

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Modern C++ öncesinde bir sınıf nesnesini containerda tutabilmek için copy ctorunun olması gerekiyordu.Çünkü contaişnera koymanın yolu kopyalamaktan geçiyordu.
  O dönemde move semantics te yoktu. Şimdi modern C++ ile birlikte artık copy ctor tek yok değil. Çünkü emplace funclarla containerın sağladığı bellek alanında containerı hayata getirebilirim.

- Copy ctor olmasada move ctorun kullanılmasını sağlayabiliriz. Tek yapmamız gereken rvalue expr kullanmak.

Move only tür tipik olarak unique_ptr nesnesi. Bunun üzerinden ilerleyeceğiz.

Burada unique_ptr çok kısaca anlatıldı. Ben yazmadım o yüzden Temel C++ ta ayrıntılı görüldü.


Sink functionu
Herb sutter scott meyers tartışma olayı


#include <vector>
#include <string>
#include <memory>


HERB SUTTER VERSION

using usptr = std::unique_ptr<std::string>;

void sink(usptr uptr) // bu function herzaman mülkiyeti alacak çünkü mnove only type parametreli
{

}

int main()
{
	auto ptr = make_unique<string("Necati ergin")>; 
	sink(ptr); // SENTAKS HATASI olur çünkü kopyalamaya karşı kapalı
	sink(std::move(ptr)); // GEÇERLI. BUrada taşıma semantiği devreye girecek ve nesnenin kaynağını çalacak. ptr mülkiyeti bırakacak. functaki uptr kaynağı alacak
							// function bloğundan çıktığı an o da kaynağı bırakacak.
}


SCOTT MEYERS VERSION

using usptr = std::unique_ptr<std::string>;

void sink(usptr &&uptr) // DİKKAT!! Func rvalue expr ile çağrılacak ama nesne sadece referansa bağlandı kaynağı alınmadı.
{

}

int main()
{
	auto ptr = make_unique<string("Necati ergin")>;
	
	sink(std::move(ptr)); 
}


BAŞKA BİR MODEL DAHA VAR

using usptr = std::unique_ptr<std::string>;

usptr func()
{
	std::unique_ptr<std::string> uptr{new std::string{"alican"}};

	return uptr; //GEÇERLI. Otomatik ömürlü nesneler return edildiğinde lvalue to rvalue expr dönüşümü uygulaniyor.Return value optimization değil

	// return std::move(uptr); // BU HATA!!!!!!!
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

using usptr = std::unique_ptr<std::string>;

void func(usptr x)
{

}

int main()
{
	usptr up;

	func(make_unique<string>("ali")); // Geçerli zaten bir rvalue expr bu.

	func(new string("ali")); // Sentaks hatası çünkü unique ptr nin T* parametreli ctorunun explicit olması

	usptr up = new string; // Yine ctor explicit olduğu için geçersiz. Explicit olunca copy init sentaksı gerçerliliğini kaybediyor.

}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


MOVE ONLY SINIFLARIN 2 TANE KISITLAMASINA DIKKAT.

1. Eğer bir container move only type tutacaksa artık containerın initializer list parametreli ctorunu kullanma ihtimali yok.

int main()
{
	unique_ptr<string> x,y,z;

	vector<unique_ptr<str>> vec{std::move(x),std::move(y),std::move(z)}; // move etsek dahi SENTAKS HATASI
}


MOLA
=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================
=============================================================================================================================================================================================

using usptr = std::unique_ptr<std::string>;

int main()
{
    auto uptr = std::make_unique<std::string>("dogukan");

	std::vector<usptr> vec{usptr{new std::string}, std::make_unique<std::string>("Necati")}; //Burada da init list var .SENTAKS HATASI
    //Move only sınıfları tutan containerların init list parametreli constuctorlarını çağırmak sentaks hatası

    ----------------------------------------------------------------------------------------

    std::vector<usptr> vec(100); //GEÇERLİ. 100 nesneyi default init ettik

    std::vector<usptr> vec(100,uptr); //SENTAKS HATASI. Kopyalama yapmak zorunda bu. Bu sebeple sentaks hatası

    ----------------------------------------------------------------------------------------

    Aşağıdaki vectore hangi biçimlerde move only type öğe insert edebilirim?
    std::vector<usptr> vec;

    1. Vectorün sağ taraf referans parametreli pushback func var. dolayısıyla pushback e rvalue expression ile
        bir unique ptr geçebiliriz.
        
        auto uptr = std::make_unique<std::string>("dogukan");

        std::vector<usptr> vec;

        vec.push_back(std::move(uptr));

    2. Return değeri bir unique ptr olan func a yapılan bir çağrı ifadesini pushback e argüman olarak gönderebiliriz.

        vec.push_back(std::make_unique<std::string>("Mustafa"));
        
    3. Bir geçici nesne ifadesi oluşturabiliriz.

        vec.push_back(usptr{new string("Gokhan")});

    4. Emplace olur

        vec.emplace_back(); // İlginç burası. Bu şekilde yazıldığında default ctor çağrılır hata yok.

        vec.emplace_back(new string("Taylan")); //GEÇERLİ.Containerdaki öğenin ctoruna gönderilecek öğe yazılır emplace te


    ------------------------------------------------------------------------------------------------------------

    Diyelimki biz bu vectoru dolaşacağız ve vectordeki unique ptr lerden dolu olanların gösterdiği kontrol ettiği
    nesnelerin değerlerini yazdıracağız.

    for(auto &up : vec) // auto up : vec yazarsak sentaks hatası çünkğ kopyalama yapılmaya çalışır bu
    {                   // auto &&up : vec te ise sorun yok.Yİne geçerli olur
        if(up)
        {
            std::cout << *up << "\n";
        }
        else
            std::cout << "Bos\n";
    }

    ------------------------------------------------------------------------------------------------------------

    SENTAKS HATASI VAR MI?
    vectordeki öğeleri listeye kopyalayabilir miyiz ?

    list<uptr> mylist{vec.begin(),vec.end()}; //SENTAKS HATASI BURASI.Çünkü kopyalamaya karşı kapalı burası

    GEÇERLİ NASIL OLABİLİR?
    MOVE_ITERATOR. iterator header file içinde

    Aşağıda vectordeki öğeler listeye taşındı.Ama vector boşalmadı.Size yine aynı ama vectorde dolu olan
    unique_ptr ler şimdi boş hale geçti.Listeye taşındılar çünkü
    list<uptr> mylist{make_move_iterator(vec.begin()),make_move_iterator(vec.end())};

    ------------------------------------------------------------------------------------------------------------

    MAKE_MOVE_ITERATOR NE YAPIYOR ?

    vector<string> svec{"ali","veli","hasan"};
    auto iter = make_move_iterator(svec.begin());

    string str = *iter; // bunu * nin operandı yapınca aslında movefunctiona argüman olarak gönderiliyor.
                        // *iter aslında move un return değeri demek normal iteratorden farklı olarak.Kod buna
                        // göre yazılmış
    std::cout << "str = " << str << "\n";

    std::cout << "*iter = " << *iter << "\n";

}

---------------------------------------------------------------------------------------------------------

 MOVE ALGORİTMASI ÖRNEĞİ

 int main()
 {
    vector<string> svec;
    rfill(svec,100,rname);
    print(svec);

    list<string> slist(100);
    //vectoru listeye kopyalamak istesem copy algoritmasını kullanırım
    copy(svec.begin(), svec.end(), slist.begin());

    //Soru farklı, vectordeki öğeleri listeye taşımak isteseydik ne yapardık? MOVE ALGORİTMASI
    move(svec.begin(), svec.end(),slist.begin());

    // copy ve copy_backward aynı zamanda move ve move_backward algoritmaları var.

 }

 Copy nin template kodu

 template<typename Initer, typename Outiter>
 Outiter Copy(Intiter beg, Initer end, Outiter destbeg)
 {
    while(beg != end)
    {
        *destbeg++ = *beg++;   // MOVE ise bu adımda *destbeg++ = std::move(*beg++); Yapıyor.
    }
    return destbeg;
 }


 Move Algoritmasının kodu
 template<typename Initer, typename Outiter>
 Outiter Move(Intiter beg, Initer end, Outiter destbeg)
 {
    while(beg != end)
    {
        *destbeg++ = std::move(*beg++);
    }
    return destbeg;
 }

---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------
 NOT !!!!!!!!!!!!!!
BUNLARIN DIŞINDAKİ FUNCLAR İÇİN YANİ DOĞRUDAN BU ALGORİTMA YOKSA
YAZMA ALGORITMASINI COPY YERINE MOVE YAPMAK ISTERSEK MOVE_ITERATOR KULLANMALIYIZ.
 MOVE ITEARTOR KULANINCA ITERATOR KONUMUNDAKI NESNEYI BIRDEN FAZLA KERE KULLANIYOR OLMAMALI ALGORITMA
 EĞER ITARATOR KONUMUNDAKI NESNEYI BIRDEN FAZLA KEZ KULLANMA RISKI VARSA O ZAMAN TAŞINMIŞ NESNEYI
 KULLANMA RISKI OLACAK. INPUT ITERATOR KATEGORISININ BU GARANTISI VAR ZATEN.
---------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------

COPY_BACKWARD
Sondan başlayarak kopyalama işlemi yapıyor.

 int main()
 {
    vector<string> svec;
    rfill(svec,10,rname);
    print(svec);

    copy_backward(svec.begin(), svec.begin() + 6, svec.end()); //random access olduğundan + işlemi yapılabilir.
    print(svec);    // svec ile svec[6] arasındaki öğeleri tersten başlayarak, svecin end inden başlayarak geriye
 }                  // doğru ekliyor.


TAŞIYANI İSE MOVE BACKWARD
move_backward(svec.begin(), svec.begin() + 6, svec.end()); //random access olduğundan + işlemi yapılabilir.

Aradaki tek fark copy yerine move kullanılıyor. Eğer vectorde tutulan öğeler move only type ise copy kullanıldığında
sentaks hatası olacak. Move algoritmasını kullandığımızda ise sentaks hatası olmayacak.

int main()
{
    vector<unique_ptr<string>> svec;

    for(int i = 0; i < 10; ++i)
    {
        svec.emplace_back(new string{"rname()"});
    }

    list<unique_ptr<string>> slist(10); // burada range ctor kullanılabilirdi ama biz başka türlü yapmak istiyoruz.
                                       // copy ve move örneklemek için yapıyortuz.

    NOT: Burada list<unique_ptr<string>> slist; default init edildiğinden yeterince öğe yok içerisinde
        Bu sebeple ya back_insert iterator ya front_insert itrator yada insert iterator kullanmamız gerekiyordu.
        Yada eleman sayısını baştan vereceğiz.


    // copy(svec.begin(), svec.end(),slist.begin()); // SENTAKS HATASI
    move(svec.begin(), svec.end(),slist.begin()); // GEÇERLi.Artık vectordeki elemanlar boşa çıktı.

    for(auto &up : svec)
    {
        if(up)
            std::cout << *up  << " ";
        else
            std::cout << "bos\n";
    } /// bos bos bos ....

    for(auto &up : slist)
    {
        if(up)
            std::cout << *up  << " ";
        else
            std::cout << "bos\n";
    } // isimler yazılacak
}

Containerdan herhangibir öğe de taşıyabilirdik. Dİkkat!!! Bu containerdan o öğenin silindiği anlamına gelmiyor.
Container size ı değişmeyecek yani.

 auto ptr = std::move(svec[5]); // svec[5] bir unique ptr nesnesi ve sağ taraf değerine çevirdik.
                                // bu durumda uptr için move ctor çağrıldı. DOlayısı ile uptr dolu ama
                                 // vectorun 5 indisli öğesi boşa çıktı.

 cout << (svec[5] ? "not empty" : "empty"); / boş çıkar. Main içinde yazdı hoca ben orada yazmadım bunu.


------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------

int a[](1,2,3); C++ 20 ile geçerli. C++17 de hata
int a[]{1,2,3}; GEÇERLİ

------------------------------

 struct data{
    int x,y,z;
 };

 int main()
 {
    Data mydata(1,2,3); //C++17 SENTAKS HATASI, C++20 GEÇERLI
 }

*/
