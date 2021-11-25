/*

 IOSTREAM sınıfları move only sınıflar. kopyalamaya karşı kapalı. Kopyalama gereken yerde kullanılamıyor.
 Mesela bind adaptörünü kullanırken kopyalama yapılamadığı için ref wrapper kullanmıştık.

 Taşıma semantiğinden faydalanarak bir functan bir func a bir iostream nesnesini taşıyabiliyoruz.

 #include <fstream>

 std::ofstream open_write_file(const std::string& filename) // return türü & değil doğrudan tür
 {
    std::ofstream ofs{std::move(filename)};
    if(ofs.fail())
    {
        std::cerr << "cannot create file\n";
        std::exit(EXIT_FAILURE);
    }
    return ofs; // Move only ama return ettik. Lvalue to rvalue conversiondan dolayı geçerli.
 }

 void write_file(std::ostream ofs)
 {
    ofs << "bugün hava soğudu\n";


 }

 int main()
 {
    using namespace std;

    auto ofs{open_write_file("alikalan.txt")};

    write_file(ofs); // burası sentaks hatası olacak çünkü kopyalamaya karşı kapalı bu sınıf

    write_file(std::move(ofs)); // Function parametresine ofsteam nesnesini taşıdık ama bu func çağrıldığı zaman
                                // func çalışmasının kodu bittiğinde closing brace e geldiğinde otomatik ömürlü
                                // değişken ofs için scope sonu gelecek ve bu nesne için destructor çağrılacak.
                                // ve dosya kapatılmış olacak.
        
 }

BUNLARI NEREDE KULLANACAĞIZ?

 int main()
 {
    using namespace std;

    ofstream {"alikalan.txt"}; bu bşr geçici nesne
    
    ofstream {"alikalan.txt"} << "C++ is a complex language\n"; // bu geçerli.Geçici nesne evaluate edilecek ve  yine
                                                                // geçici nesnenin hayatı bitecek ve bittiğinde
                                                                // dtoru çağrılacak ve dtoru çağrıldığında dosya
                                                                // kapatılacak.
 }


---------------------------------------------------------------------------------------------------------------

 int main()
 {
    open_write_file("alikalan.txt") << "it is interesting!";
 }

---------------------------------------------------------------------------------------------------------------

NOT : Otomatik ömürlü nesneyi return ettiğimizde orada bir tür dönüşümü oluyor. Derleyici gözüyle bakınca return ifadesi LVALUE olmaktan çıkıp XVALUE ya dönüşüyor.Bu dilin kuralı
Dolayısı ile move ctor çalışacak.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

int main()
{
    std::string str{"yigit polat"};
    std::string name,surname;

    // std::istringstream{str}; burası bir rvalue expression aşağıda kullanacağız
    std::istringstream{str} >> name >> surname; // C++11 ile birilkte artık burası çalışıyor.

    std::cout << "name = " << name << "\n";
    std::cout << "surname = " << surname << "\n";

}

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LIFE EXTENSION DURUMLARI
------------------------

Life extensionun geçerli olduğu durumlar.

std::string foo();
std::string func(const std::string &s);

int main()
{
    foo(); // ifade rvalue expression

    const std::string &s = foo(); // bu nesnenin ömrü referans scopuna endexlendi.
                                   // bunu başka bir şekilde başka referansa aktaramayız.
    
    std::string &&s = foo(); // burada yukarıdaki ile life extension açısından fark yok.

    func(std::string("Ali")); // function argümanı life extension oluyor.

}

========================================================================================================================================================================================================================
========================================================================================================================================================================================================================
========================================================================================================================================================================================================================
========================================================================================================================================================================================================================

LAMBDA İFADELERİ - PROFESYONEL SEVIYEDE
---------------------------------------
Her standartta birşeyler katıldı lambda ifadelerine.11 14 17 20 standartlarında sürekli ekleme yapıldı.
C ve C++ ta copiler extensio kullanmazsak nested function yok. Nested function olsaydı belkide lambda ifadeleri dile eklenmeyecekti.
Lambda ifadeleri öyle ifadeki bu ifade karşılığı bir sınıf kodu oluşturuyor. İfadeyide o sınıf türünden bir prvalue expressiona döndürüyor.

Derleyicinin lambda ifadesi karşlığı oluşturduğu sınıfa closure type deniyor.
Bİr çok programcı doğrudan closure diyor. Sınıf türünden nesneyi ifade edersek 
closure hem türün hemde nesne karşılığı olarak kullanıyoır.Ama bunlardan biri
closure type diğeri closure object.

Derleyicinin oluşturduğu bu sınıfın doğrudan kodda bir ismi yok ama derleyicinin id olarak
kullandığı bir isim var. Böylece derleyiciyue bir sınıf yazdırmış oluyoruz ve bu sınıf aslında bir
functor sınıf. Çünkü compiler oluşturduğu sınıf için function call operator func yazıyor
ve duruma göre sınıfın special member funclarını yazıyor(standartlara göre).

Hatta duruma göre derleyici olutşruduğu sınıf için bir type cast operator func yazıyor.
Nedir bu type cast operator func, ilgili sınıf türünden bir func pointer türüne dönüşüm yapan
bir function yazıyor.


                    ->type
                    mutable
[] <typename T> ()  constexpr   { kod }
                    noexcept

[] = lambda introducer deniyor.Capture block ta kullanılan diğer terim
() = derleyicinin yazacağı functor sınıfın, func call operator func ının parametre parantezi.Yazımı opsiyonal
     Derleyicinin yazacağı func ın parametre değişkeni yoksa ve yukarıdaki ->type,mutable ... niteleyicileri olmayacak(4 niteleyici)
{} = Bu bloğun içinde kodlar olacak.Bu kodun neler içereceği konusunda başlangıçta çok kısıtlı imkanlar vardı.
     Her standartlarda gevşetildi.


->type = Trailing return type.Compiler functionunun return değeri türünü belirtme hakkımız var.
         Belirtmezsek derleyici func ın return ifadesine bakarak deduction yoluyla çıkarım yapacak

mutable = Derleyicinin yazdığı member func const member func. Fakat bazı durumlarda compiler yazdığı member funcların
const olması işimize gelmiyor. Çünkü derleyicinin yazdığı closure typeın veri elemanları olduğunda const member func içinde
a - O veri elemanlarını değiştiremiyoruz.
b - Bu veri elemanları sınıf türündense, onların const olmayan member funclarını çağıramıyoruz.
    Sadece const member funclarını çağırabiliriz.

constexpr = Functionun constexpr olduğunu gösteriyor. Bunu kullanmazsakta constexpr koşullarını sağlıyor ise defaultta constexpr kabul ediliyor.
            Constexpr koşullarını sağlamamasına rağmen bu keywordü kullanırsak o zaman sentaks hatası olacak.


Noexcept = Bu default değil. Derleyicinin yazdığı sınıfın func call operator funcı/lamba funcı da denebilir bu function noexcept değil.
            Noexcepti sağlıyor olsada defaultta noexcept değil.Biz bunu belirtebiliriz.

<typename T>  = Burasıda C++ 20 ile geldi.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    [](){}(); // bu geçerli. :D
               // {} içine birşey yazmadık kodu yok. return statement yoksa return değeride void

    ((((([]() {})))))(); // buda geçerli :D:D

    ((((([]() {{{{{}}}}})))))(); // burasıda halen geçerli

}

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



int main()
{
    cout << [](int x){ return x * x + 5;}(10) << "\n"; // çağırabiliriz.

    ------------------------------------------------------------------------------------------------------------------------------------------

    auto f = [](int x){ return x * x + 5;}; // tür çıkarımı için auto kullandık.
                                            // f bildiğimiz isimlendirilmiş otomatik ömürlü bir sınıf nesnesi.
    
    f(20); // çağırabiliriz.
    
    ------------------------------------------------------------------------------------------------------------------------------------------

    std::Cout << f(10) << "\n";
    std::Cout << f(20) << "\n";
    std::Cout << f(30) << "\n";
    
    ------------------------------------------------------------------------------------------------------------------------------------------

    std::invoke(f,10); // böylede çağrılabilir. 

    ------------------------------------------------------------------------------------------------------------------------------------------

    decltype(f); // closure type ın kendisi

    decltype(f) x; // C++ 20 de bu geçerli. 
    C++20 den önce lambda ifadelerinde derleyicinin olutşrduğu sınfın default ctor delete edilmişti.
    C++20 itibariyle Capture içermeyen stateless lambdaların default ctorları var.
    Aynı şekilde copy assignment delete ediliyordu artık C++20 ile stateless lambdalarda copy assignment delete edilmiyor.

    ------------------------------------------------------------------------------------------------------------------------------------------

    typeid(decltype(f)).name(); // name in return ettiği yazı derleyiciye bağlı.Microsoft derleyicisi baya betimleyici
    bir yazı yazdırıyor.Hem türün isminin ne olduğı derleyiciye bağlı hemde name func ın ne döndüreceği
    derleyiciye bağlı.
}
------------------------------------------------------------------------------------------------------------------------------------------

İLERLEYEN DAKIKALARDA DETAYLI GÖRÜLECEK
Lambda ifadesi karşılığı oluşturulan türünü kullanmak için c++20 öncesinde isimlendirilmiş nesne gerekiyordu
ki decltype a onu operand olarak gönderebileyim.C++ 20 ile gelen en önemli eklentilerden biri lambda ifadeleriyle ilgili,
lambda ifadeleri unevaluated context te kullanılabilir.

decltype operandı olan ifade unevaluated context oluşturduğu için lambda ifadesinin kendisini decltype operandı
yapabiliriz. Doğrudan bu türü elde etmek için isimlendirilmiş nesne gerekmiyor. STL de mucizeler yaratabilir.

Closure type le ilgili en önemli noktalardan biri, özdeş lambdalar birden çok yazılsa bile, kodlar aynı olsa bile
farklı farklı sınıflar oluşturmak zorunda. Bu diln kuralı.

auto f1 = ()[] {return 1;}
auto f2 = ()[] {return 1;}
auto f3 = ()[] {return 1;}

Bunların hepsi ayrı sınıf türünden nesneler.

std::cout << (typeid(f1).name() == typeid(f2).name()); // false verir

aynı şekilde is_same type trait i de kullanabiliriz
cosntexpr auto b = is_same_v<decltype(f1),decltype(f2)>;

------------------------------------------------------------------------------------------------------------------------------------------

STD::FUNCTIONA İLE TUTULABİLİR.

std::function<int(int)> f [](int x){return x * x};

------------------------------------------------------------------------------------------------------------------------------------------

BİR FUNCTİONA ARGÜMAN OLARAK GÖNDERİLEBİLİR

Func parametresinin func şablonu yada std function olmalı
STL de algoritmalara argüman olarak gönderiliyor ve bu aşırı çok kullanılıyor.

using namespace std;

int main()
{
    vector<string> svec;
    rfill(svec, 100,rname);
    print(svec);
    
    size_t len;
    std::cout << " uzunluk girin: ";
    std::cin>>len;

    //predicate isteyen 1-2 algoritmaya bakalım
    if(auto iter = find_if(svec.begin(), svec.end(),
        [len](const std::string &s){return s.length() == 5;}); iter != svec.end())
    {
        std::cout << *iter << " " << distance(svec.begin(), iter) << " indeksli eleman\n";            
    }
    else
    {
        //iteri burada da kullanabiliriz
        std::cout << "bulunamadi\n";
    }

}

----------------------------------------------------------------------------------------------------------------

    SORU ÜZERİNE

    if(auto iter = find_if(svec.begin(), {}, // burası legal
        [len](const std::string &s){return s.length() == 5;}); iter != svec.end())

----------------------------------------------------------------------------------------------------------------

Lambda ifadesini std::move a argüman gönderebiliriz.

----------------------------------------------------------------------------------------------------------------

MOLA

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================

class Myclass{
public:
    Myclass();
    Myclass(int);
};

template <typename T>
void func(T x, T y);
 
 int main()
 {
    Myclass m;
    func(m, {}); //? NEDEN GEÇERLİ
 }

-------------------------------------------------------------------------------------------------------------------------------------------

 int main()
 {
    Myclass mx = 10; // Burada C.Ctor sayesinde dönüşüm yapılıyor.Bu Ctor explicit olsaydı type cast yapmamız gerekecekti.
 }

-------------------------------------------------------------------------------------------------------------------------------------------

class Myclass{
public:
    explicit Myclass(); // tuhaf durum ama default ctor explicit yapabiliriz.
    Myclass(int);
};

template <typename T>
void func(T x, T y);

int main()
{
    Myclass mx = {}; // Şuanda SENTAKS HATASI
}

-------------------------------------------------------------------------------------------------------------------------------------------

class Myclass{
public:
    Myclass(); // tuhaf durum ama default ctor explicit yapabiliriz.
    Myclass(int);
};

template <typename T>
void func(T x, T y);

int main()
{
    Myclass m;
    func(m, {}); // Legal çünkü 1. parametre için deduction yapılacak.T myclass olarak alınacak.
}

-------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    vector<string> svec;
    rfill(svec, 100,rname);
    print(svec);

    sort(svec.begin(), svec.end()); // functor olarak std::less kullanıyor.Küçükten büyüğe sıralanıyor.
    
    generate 
    transform ... hepsinde callable var.

    generate(ivec.begin(), ivec.end(), []{return rand() % 1000;});
    
    sort(ivec.begin(), ivec.end(), [](const string& s1, const string& s2)
        { return s1.lengt() < s2.length() || (s1.length() == s2.length() && s1 < s2); });
        //Çok uzun lambda ifadesi
        //Özel bir karşılaştırma kriteriyle kıyaslandı. Kısa olanlar başta ve onlarda kendi içinde
        //lexicographical olarak kıyaslandı
}


-------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------

TÜR ÇIKARIMI

Functionun return type ı eğer trailing return type kullanılmazsa return ifadesinden deduce ediliyor.

[](int x){ return x + 1; };

Tür çıkarımı return değeri ifadesinden çıkarım çok daha fazla kullanılıyor.Bazı durumlarda trailing return type kullanılarak çıkarım yapılmasını beklemeyip
return değeri türünün bizim istediğimiz tür olmasını sağlıyoruz.

[](int x)->double{ return x + 1;}

-------------------------------------------------------------------------------------------------------------------------------------------

Trailing Return type kullanılan durumlar

1 - Func içinde birden fazla return statement varsa fakat bunların türü farklıysa 
bu durumda derleyicinin çıkarım yapma şansı yok

[](int){
    if(x > 10)
        return 1;
    
    return 2.3;
};

Burada int mi olacak double mı ? Sentaks hatası oluyor trailing return type kullanmazsak.

NOT !!!!!!!!!!
 ->type
 mutable
 constexpr
 noexcept   Bunları kullanırsak () bu paramere parantezini yazmamız şart.
 
-------------------------------------------------------------------------------------------------------------------------------------------

2 - Return değeri türünü belirtmezsek return değeri türü auto nun kurallarına göre belirleniyor.

int &foo();

int main()
{
    auto f = [](){
        return foo();        
    };

    f(); // foo yukarıda referans döndürmesine rağmen auto ile deduction yapıldığından reflik gitti
    f() = 23; // burası bu sebeple sentaks hatası olur.Constluk olsaydı o da giderdi.Auto deduction basic kurallar.
}


Bunu korumak için decltype auto kullanırız.

int &foo();
int main()
{
    auto f = []() -> decltype(auto){
        return foo();
    };

    f() = 23; // Bu artık geçerli 
}

========================================================================================================================================
========================================================================================================================================
========================================================================================================================================

LAMBDA INTRODUCER []

Bu köşeli parantezin içi hayati öneme sahip. Çünkü içi boş bırakılabileceği gibi burada bir capture close olabilir.

Capture close = Lambda ifadeleri karşılığı derleyici bir sınıf kodu yazıyor.Aşağıdakini düşünelim.
Bu sınıfın veri elemanını kullanabilriiz. Derleyiciye bu sınıfı yazdırmak istiyoruz.
Derleyiciye yazdıracağımız sınıfında veri elemanı veya elemanları olabilsin hatta biz istersek
referans olsun yada olmasın demek istiyoruz. Capture Block böyle bir sınıf yazdırmayı sağlıyor.

class Functor{
public:
    Functor(int val) : mx {val} { }
    void operator()(int x)
    {
        mx;
    }

private:
    int mx;
};


KURALLAR
--------

Local classlar ile classlar arasında ufak farklılıklar var.

int main()
{
    int x;
    class Myclass{
        void func()
        {
            x = 10; // SENTAKS HATASI !!!!!!
        }
    };
}

--------------------------------------------------------------------------------

DİKKAT!!!!!
    
int x;
int main()
{
    //static int x; olsaydı yine GEÇERLİYDİ
    class Myclass{
        void func()
        {
            x = 10; // GEÇERLİ !!!!!!
        }
    };
}

--------------------------------------------------------------------------------

Buradaki kurallar local classlardan gelen kurallar. Neden static nesneleri capture etmediğimiz belli.
Static ömürlüler zaten visible durumda.


int x = 23; // ilgili blokta olsaydı SENTAKS HATASI

int main()
{
    auto f = [](int a){ return x + 5;}
} 

--------------------------------------------------------------------------------

Lambda func içinde ilgili blokta visible olan static ömürlü değişkenleri ve ilgili namespace teki değişkenleri
doğrudan kullanabiliyoruz. Hatta capture etmeye çalışsak sentaks hatası olur

Derleyiciye öyle sınıf yazdırmak istiyoruzki, derleyici o sınıf için bir ctor yazdıracak ve yazdırdığı ctorda
x i ctorın parametresine argüman olarak gönderecek ve sınıfa bir veri elemanı koyacak ve bu ctor bu yerel değşkenle
initialize edilecek.

int main()
{
    int x{}; 
    auto f = [x](int a){ cout << a << "\n";};
}

ör:
int main()
{
    int x = 10, y = 20;
    double d = 5.6;
    auto f = []{return 1;};

    std::cout << sizeof(decltype(f)) << "\n"; // Closure type storage değeri. 0 olamaz bu 1 olacak.

    ------------------------------------------------------------
    
    auto f = [x]{return 1;}; // Şimdi sizeof 4

    ------------------------------------------------------------
    
    auto f = [x,y]{return 1;}; // Şimdi sizeof 8

    ------------------------------------------------------------
    
    auto f = [x,y,d]{return 1;}; // Şimdi sizeof 16

    Derleyicinin yazdığı sınıfta bunlar gerçekten nonstatic veri elemanları

}

Değişkenleri copy yolula capture edeceksem copy capture deniyor.
Derleyici benim için sınıf yaz sınıfın veri elemanı bu türden olsun ve buna bu ifade ile ilk değer ver demek.
virgüller ler ile ayrılan liste ile n tane isim yazılabilir.

Mülakat sorusu

class Nec{
public:
    Nec(int &r) : mr{r} { }
private:
    int &r;
};

Ostream iterator sınıfıda ostream nesnesini referans yoluyla alıyor.

Derleyiciye böyle bir sınıf yazdırmak istersek te sentaksı aşağıda.

int main()
{
    int x = 10, y = 20;
    double d = 5.6;
    auto f = [&x]{x++;}; // x i değil bağlandığı nesneyi değiştiriyorum.Bu sebeple mutable yazmaya gerek yok. Şaşırılan nokta burası

    f();  // x 11 oldu

    std::cout << "x = " << x << "\n";
    
    ----------------------------------------------------------------------------

    auto f = [&x,y](){x++;}; // Yazılabilir
    
    auto f = [&x,y,&d](){x++;}; //  Yine yazılabilir.

    ----------------------------------------------------------------------------

    Diyelimki functionun ana bloğu içinde capture ifadesi olarak yazdık ama functionun ana bloğu içinde kullanmadık.
    Bu durumda yine derleyici sınıfın bu veri elemanlarını koyuyor.
    auto f = [x,y,d](){ }; 

    std::cout << sizeof(decltype(f)) << "\n"; // 16

}

-------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------

GÖRÜNÜR NE VARSA LAMBDA IÇINDE KULLANMAK ISTERSEK
Her nesneyi copy yoluyla capture etmek [=]

int main()
{
    int x = 10, y = 20; 
    double d = 5.6;

    auto f = [=](){ return 1; }; //dikkat
    std::cout << sizeof(decltype(f)) << "\n"; // 1 ÇIKTI !!!!!!

    ----------------------------------------------------------------------

    auto f = [=](){ return x; }; //dikkat !!!
    std::cout << sizeof(decltype(f)) << "\n"; // 4 ÇIKTI !!!!!!
    
    ----------------------------------------------------------------------

    auto f = [=](){ return x + y + d; }; //dikkat !!!
    std::cout << sizeof(decltype(f)) << "\n"; // 16 ÇIKTI !!!!!!

}

BURADA = İLE CAPTURE EDİLENLER, KULLANMASAK SIZEOF A DAHIL DEĞIL. 
TEK TEK CAPTURE EDINCE HEPSI SIZEOF A DAHIL OLUYORDU

-------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------

int main()
{
    int x = 10, y = 20;
    double d = 5.6;
    
    auto f = [=]()mutable { ++x; ++y; d*= 10.5; };

    f();
    
    std::cout << "x = " << x << "\n"; // 10
    std::cout << "y = " << y << "\n"; // 20
    std::cout << "d = " << d << "\n"; // 5.6
}

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

REFERENCE CAPTURE [&]

int main()
{
    int x = 10, y = 20;
    double d = 5.6;

    auto f = [&]() { ++x; ++y; d*= 10.5; };

    // f yi de capture edeceği için aşağıdaki capture ifadesi içinde kullanabiliriz.
    // En sık kullanılan idiyomlardan biri.
    [=](){};
    std::cout << "x = " << x << "\n"; // 11
    std::cout << "y = " << y << "\n"; // 21
    std::cout << "d = " << d << "\n"; // 58.8
}

-----------------------------------------------------------------------------

ÖR:
int main()
{
    string str;
    vector<string> ivec(10);

    [&](){
        ivec.push_back(str); // geçerli.yukarıdaki ifadelerde callable olabilirdi.
    };
}

-----------------------------------------------------------------------------

Referans yoluyla capture etmek istiyoruzç ama lambda içerisinde bu nesneyş yanlışlıkla değiştirmeye
karşıda önlem almak istiyorum. std::as_const ile yapıyoruz. Sentaksı temelde gösterildi.

class Biggie{

};

int main()
{
    Biggie x;

    auto f = [&x](){
        x.set();
    };
}

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------

Referans ile cağture etmenin avantajları var. Kopyalama maliyeti yok birde nesneyi değiştirebiliyoruz.

#include <iostream>
#include <memory>

using namespace std;

int main()
{
    unique_ptr<int> uptr{ new int(12)};

    [uptr](){  //SENTAKS HATASI. REFERANS YOLUYLA CAPTURE ETMEK GEREK
        if(uptr)
            std::cout << *uptr << "\n";
    };

    ÇOK DIKKATLI OLUNMASI GEREKEN DURUM DANGLING REFERENCE OLABILIR !!!!!!
}

-------------------------------------------------------------------------------------------

UNDEFINED BEHAVIOR ÖRNEĞİ

auto func(int x)
{
    return [x](int a){ return a * x; };
}

auto foo(int x) // UNDEFINED BEHAVIOR
{               // X OTOMATİK ÖMÜRLÜ NESNE VE ÖMRÜ BİTİYOR AMA AŞAĞIDA REFERANSI RETURN EDİLMİŞ
                // MAIN İÇİNDE DE AYNI NESNE KULLANILIYOR G OLARAK HALEN UB.

    return [&x](int a){ return a * x; };
}

int main()
{
    auto f = func(12);
    std::cout << "f(20) = " << f(20) << "\n";  // 240

    auto g = func(12);
    std::cout << "g(20) = " << g(20) << "\n";  // 400 UNDEFINED BEHAVIOR
}

-------------------------------------------------------------------------------------------

CAPTURE YOLLARI

int main()
{
    int x{}, y{}, z{};

    auto f = [=, &x]
    
    auto f = [=, &x]() {}; // x i refernece diğerlerini copy yoluyla
    
    auto f = [=, &x,&y]() {}; // x ve y refernece diğerlerini copy yoluyla
    
    auto f = [&, x]() {}; // hepsini refernece ile x i copy yoluyla

    TEMEL C++ TADA GÖRÜLDÜ

    ---------------------------------------------------------------------------------------

    int x = 10;

    auto f = [x]() {
        auto a = this->x; // NEDEN SENTAKS HATASI OLDU   
    };                    // Lambda ifadelerine ilişkin bir kural. member func içinde this
                          // kullanmak sentaks hatası olur.
                          // Thise daha faydalı bir anlam alanı yarattıkları için böyle
                          //MOLA
                          //Bir sonraki ders görülecek sebebi.


}

MOLA
2:05

======================================================================================================
======================================================================================================
======================================================================================================
======================================================================================================

THIS IN LAMBA İÇİNDEKİ KULLANIMINA GELECEĞİZ

C++14 İLE LAMBDA İNİT CAPTURE GELDİ

int main()
{
    int x;

    [a = x + 5]() {}; // a yine sınıfın veri elemanı ama hayata x + 5 ifadesi ile init edilecek.

    [&a = x](){ };  // Şimdi lambda func içinde a ismini kullanınca a yine sınıfın veri elemanı olan referans
                    // x ile initialize edilmiş olacak.

    -------------------------------------------------------------------------------------------

    NAME LOOKUP ENTERESAN BURADA

    [x = x](){}; // Soldaki x sınıfın veri elemanı, sağ taraftaki x ise blok içinde aranıyor.
                    // isim arama burada farklı. Burada genelde x = x + 5 kullanılıyor

    -------------------------------------------------------------------------------------------
    
    EN ÇOK KULLANILAN MOVE ONLY NESNEYİ SINIFIN VERİ ELEMANINA TAŞIMAK

    unique_ptr<int> up{new int{76}};
    
    [up](){}; // BUrada sentaks hatası olur copy ye karşı kapalı

    [&up](){}; // Burada referans yoluyla capture edecek ama istediğimiz bu değil.

    -------------------------------------------------------------------------------------------
    
    // Sınıfın veri elemanına taşımak istiyoruz bunu ve uptr nin boşa çıkmasını istiyoruz.

    unique_ptr<int> up{new int{76}};

    [up = std::move(up)](){}; // maindeki up kaynağını lambda içindekine devretti.

    auto f = [up = std::move(up)](){
        if(up)
            std::cout << *up << "\n";
    };

    cout << (up ? "dolu" : "bos") << "\n"; // BURADA BOŞ YAZISI ÇIKTI. 
                                            // DAHA AŞAĞIDAKI ÇAĞRI OLMADAN CAPTURE ETMIŞ BILE
    
    f();    
}

----------------------------------------------------------------------------------------------

BİR ÜSTTEKİ ÖRNEK İLE AŞAĞIDAKİ AYNI ANLAMDA DEĞİL !!!!

int main()
{
    unique_ptr<int> up {new int{76}};

    auto f = [&up](){
        auto x = std::move(up);
    };

    cout << (up ? "dolu" : "bos") << "\n"; // BURADA ŞİMDİ DOLU ÇIKAR.YUKARIDA BOŞTU

    f();    // LAMBDA FUNC ÇAĞRILDI

    cout << (up ? "dolu" : "bos") << "\n"; // ARTIK BURADA BOŞ DURUMDA
}

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

TÜR ÇIKARIMI AUTO İLE TAMAMEN AYNI DEĞİL
CAPTURE EDİLEN ELEMANIN CONSTLUĞU SINIFIN VERİ ELEMANINA YANSIYACAK.


int main()
{
    unique_ptr<int> up {new int{76}};

    const int x = 19;
    
    [x](){}; // SINIFIN VERİ ELEMANI CONSTTUR !!!!! AUTO İLE FARKLI

    -------------------------------------------------------------------------

    [&x](){}; // BURASI YINE CONST !!!!

    -------------------------------------------------------------------------

    ARRAY CAPTURE

    int a[10]{};

    auto f = [a]() mutable {   // BURADA CAPTURE EDİLEN ELEMAN DİZİ, POINTER DEĞİL.INT[10]. HOCADA CONST INT [10] çıktı
        
        a = nullptr; // POINTER OLSA ATAMA OLURDU. POINTER DEĞİL SENTAKS HATASI

        //Veya aşağıdaki kontrole bakalım

        std::cout << is_pointer_v<decltype(a)>; // 0 YANİ FALSE

        std::cout << is_array_v<decltype(a)>; // 1 YANİ TRUE

        std::cout << typeid(a).name() << "\n"; // INT[10]. HOCADA CONST INT[10] ÇIKTI
    };

    f();

    -------------------------------------------------------------------------

    ÖR:

    int a[10]{};

    auto f = [a = a]() mutable {  //ARRAY DECAY OLUR.DİZİYİ TUTARKEN POINTER KULLANIRIZ
        
        std::cout << is_pointer_v<decltype(a)>; // 1 - TRUE
        
        std::cout << is_array_v<decltype(a)>; // 0 - TRUE

        //aşağıdakide gösterir

        std::cout << typeid(a).name() << "\n"; INT * TÜRÜ
    };

    f();

}

=================================================================================================
=================================================================================================
=================================================================================================
=================================================================================================

MUTABLE
-------
Çok önemli. Öğrenenler yanlış öğreniyor.Compiler theory de keywordlerin şişirilmek istenmediği
belirtilir.Bu sebeple varolan keywordleri overload etmek daha ucuza geliyor. Mesela using
Yükle babam yükle :D:D:D:D

class Myclass{
private:
    mutable int mx; // Normalde const member funclar, bu isim mutable olmasaydı, değiştirmeye çalışsaydı sentaks hatası olurdu
};

Herkes mutable ın lambda ifadesinde de aynı anlamda olduğunu zannediyor.
int main()
{
    int x = 120;

    [x]()mutable {}; // Derleyici sen yazacağın üye functionu normalde const yazıyorsun ama şimdi
                     // memberfunc ı nonconst yaz. Bazen veri elemanını değiştirmemiz veya atama yapmamız gerekecek
                     // bu tip durumlarda nonconst olmalı func.

}


--------------------------------------------------------------------------------------------------

class Generator {
public:
    int generate();
};

int main()
{
    Generator gen;

    auto f = [gen] {return gen.generate(); }; //SENTAKS HATASI. Const member function içinden noncosnt çağrılamaz.
}   

MUTABLE OLSAYDI GEÇERLİYDİ.

--------------------------------------------------------------------------------------------------

ÇOK ÖNEMLİ SORU

int main()
{
    int x = 50;

    auto f = [&x](int a){ x *= a;}; //BURASI MUTABLE DEĞİL VE NEDEN GEÇERLİ ???
    
    std::cout << "x = " << x << "\n";
}

Burada sınıfın veri elemanını değiştirmiyoruz. Yani x i değiştirmiyoruz. x bir referans ve onun
refere ettiği nesne değiştiriliyor. Bu sebeple hata yok :D:D


AÇIKLAYICI KOD

class Myclass{
public:
    void foo()const
    {
        ptr = nullptr; //SENTAKS HATASI SINIFIN VERİ ELEMANI DEĞİŞİYOR
        *ptr = 25; //GEÇERLİ ÇÜNKÜ SINIFIN VERİ ELEMANI DEĞİŞTİRİLMİYOR.
    }
private:
    int *ptr;
};

Referans olsada aynı

--------------------------------------------------------------------------------------------------

int main()
{
    const int x = 10;

    // auto f = [x]()mutable{ ++x; }; //SENTAKS HATASI

}

=================================================================================================
=================================================================================================
=================================================================================================
=================================================================================================

CONSTEXPR
---------
Şuandaki kurallardan bahsedilecek.
Derleyicinin oluşturduğu closure typeın operator funcları eğer constexpr function olma koşullarını
çiğnemiyorsa constexpr kalıyor. Defaultu constexpr yani.


constexpr int foo(int x)
{
    static int ival = 0; // SENTAKS HATASI çünkü constexpr func içinde static yerel değişken oluşturamayız.
    //

    return x; 
}

int main()
{
    
}

--------------------------------------------------------------------------------------------

GEÇERLİ KOD AMA VISUAL STUDIO SAÇMALIYOR BURADA

class Nec{
public:
    Nec() {}
    constexpr int get()const;
};

constexpr int foo(int x)
{
    Nec mynec; //SENTAKS HATASI ÇÜNKÜ LITERAL TYPE DEĞİL BURASI.  

    return mynec.get() + x; 
}

int main()
{
    
}

CONSTEXPR OLMASI İÇİN BİR FUNC IN, PARAMETRELERİNİN, RETURN DEĞERİ TÜRÜNÜN VE 
FUNC İÇİNDEKİ LOCAL DEĞİŞKENLERİN LİTERAL TYPE OLMASI GEREKİYOR

--------------------------------------------------------------------------------------------

AYNI ŞEKİLDE GEÇERLİ KOD AMA VISUAL STUDIO SAÇMALIYOR BURADA

CONSTRUCTORU SİLERSEK VEYA USER DEFİNED DEFAULTED YAPARSAK EĞER HATA KALKIYOR
ARTIK NEC SINIFI LITERAL TYPE OLMAYA TERFİ ETTİ.

class Nec{
public:
    Nec() = default;
    constexpr int get()const;
};

constexpr int foo(int x)
{
    Nec mynec; // NEC SINIFI LITERAL TYPE ARTIK HATA YOK

    return mynec.get() + x;
}

int main()
{

}

-----------------------------------------------------------------------------------------

CONSTEXPR NITELEYİCİSİ YAZILIRSA EĞER İFADE CONSTEXPR OLMAZSA HATA OLUR .
YAZMAZSAK OLMADIĞINDA DA SORUN DEĞİL. OLURSADA DEFAULTTA ZATEN CONSTEXPR

int main()
{
    auto fsum = [](int x, int y){
        auto left = [=]{ return x; };
        auto right = [=]{ return y; };
        return [=]{return left() + right();}
    };

    constexpr auto n = fsum(10,20)(); 
}

BURADAKİ FUNCLARDA CONSTEXPR. ZATEN ATAMA YAPARKEN HATA OLURDU CONSTEXPR OLMASAYDI
TÜM FUNCTİONLARIN HEPSİNİN FUNC CALL OPERATOR FUNC I CONSTEXPR.

 */
