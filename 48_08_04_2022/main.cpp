/*

CONCURRENCY
-----------
Burada kullanılan terimler konusunda %100 bir anlaşma yok.
Buradaki terimlerin anlamı konusunda tam bir anlaşmazlıklar olsada genelde anlaşma var

Con latincede ile demek.
currency şimdiki zaman ile alakalı

Eşzamanlılık gibi diye çevriliyor. Burada kastedilen işlerin belirli bir zaman diliminde
birlikte yapılması.İş parçalarını birbirinden bağımsız halde çalışacak parçalara bölmek.

Bir programın concurrent olması, programın belirli bir zaman dilimi içerisinde işleri
parçalara ayırarak yönetebilmesi/gerçekleştirebilmesi demek. Yemek yemek ve konuşmak mesela.
Yemek yiyoruz arada konuşuyoruz, sonra tekrar yiyoruz sonra tekrar konuşuıyoruz.
Bu sayede belirli zaman dilimi içinde hem yemek yemiş hem konuşmuş oluyoruz.Bu concurrent demek.

Simultaneously diyince paralelism devreye giriyor. Burada ise fiziksel olarakta tamamen aynı anda
farklı işlerin yapılabilmesi demek.Yürürken konuşmak gibi.İki eylem tamamen aynı anda yapılıyor.

Concurrent ile paralellik arasında bir nüans var.Aralarında nasıl bir fark var.

Concurrency bağımsız yürütülen süreçlerin bir arada yürütülmesine yönelik.
Bir yandan port dinle bir yandan servera veri gönder.Birden fazla işin yapılmasını
yönetmek anlamında.

Paralelism, birbiriyle ilişkili olabilecek işlemlerin eş zamanlı olarak yürütülmesi.
Aynı zamanda birden fazla işin fiilen/fiziksel olarak yönetilmesi.
Aynı anda birden fazla işin yapılması için birden fazla çekirdek veya birden fazla
CPU gerekir.

Yada Cpu da bir core vardır fakat bu complex switcing ile belirli sürelerde farklı farklı
kodları çalıştırır. Biraz ondan biraz diğerinden ... Buradaki OS faaliyetine context switching deniyor.
Gerçek anlamda paralellik işlerin fiilen aynı yapılması. 

Paralelism olduğunda kaygı her zaman daha hızlı çalışması.
1 milyon elemanlı diziyi sıralayacağız. İşin bir kısmını 1 coreda diğer kısımlarını diğer core larda
çalıştırıyoruz mesela. İşi 4 e böl işleri farklı farklı corelara böl ve daha hızlı sıralama işlemini yap mesela.

Concurrencyde ise kaygı aynı zaman dilimi içinde yapılması gereken işlerin yönetilmesi.
Mesela orkestrada farklı farklı enstrümanların farklı farklı zamnalarda
çalışmasını sağlamak ve hep beraber aslında ortak bir iş yapmaları.Bunun yönetilmesi gerekiyor.

Paralelism, concurrency nin bir alt kümesi olarak görülebilir. Buna herkes katılmıyor.
Cuncurrency diyince en genel tanım farklı işlerin aynı zaman diliminde gerçekleştirilmesi(at the same time).
Ama simultaneously olmak zorunda değil.Yani işin bir kısmını yap sonra başka kısmını yap.

Ama paralellik söz konusu olduğunda tamamen fiziksel olarak ayrı ayrı işlerin eşzamanlı(simultaneously)
olarak yürütülmesi demek.


ŞÖYLEDE DENEBİLİR
Paralellik aynı anda birden fazla işin yapılmasıyla ilgili
Concurrency ise aynı zamanda birden fazla işin daha çok yönetilmesi ile ilgili.

Bir uygulama concurrent olabilir ama paralel olmayabilir.
Bir uygulama paralel olabilir ama concurrent olmayabilir.
Hem concurrent hemde parallelde olabilir.
İkiside olmayabilir. Yani tamamen sequencial bir şekilde.(Şimdiye kadar neredeyse hep böyle yazdık)

C++ ta başrol oyuncusu threadler.Modern C++ tan önce C++ta threadlere yönelik bir memory model yoktu.
Modenr C++ ıbn getirdiği en önemli yeniliklerden biri bir memory modelin oluşturulması ve concurrency ye yönelik araçların
standart library nin bir bileşeni olması.

Modern C++ öncesinde abstract C++ machine sanki tüm programlar single thread şeklinde çalıştırılıyor gibi oluşturulmuş.
Ama modern C++ ile birlikte birden fazla threading aynı zamanda yürütülmesine yönelik yeni tanımlar eklendi.

Eski C++ta threadler dilin bir bileşeni değildi.Multithread programlama için OS in funclarını veya  3 pary 
library kullanmak gerekiyordu. Unix linuxta pthread kullanılıyor. Bu artık standart hale geldi.
C'de de standart hale geldi. C++ ta yeni standartlar ortaya çıkınca derleyiciler tarafından hemen 
implemente ediliyor ve kullanıma giriyor. C biraz daha geriden geliyor.

Bu herşeyden önce bir taşınabilirlik getiriyor. Diğer taraftan pthread veya os apilerini kullanarak
yapılabilen herşeyi C++ concurrency modülleriylede yapamıyoruz maalesef. Daha o durumda değil ama
sürekli aradaki açıklık kapanıyor. Threadlerin standart library de bir id si var onu native handler olarak
bir functan alıp halen OS apilerinde de kullanabiliyoruz. Böyle bir faydasıda var ve giderek araç
sayısı artıyor. C++23 te eskiden yapılamayan işler artık yapılabiliyor.

NOT : Networking tarafıda standart olacak. Guid tarafıda olabilir belki.

Hoca thread ile processler ne demek bunu anlatmayacak çünkü bilindiği düşünülüyor.

Thread kullanmamızın sebebi, programın genel yapısı arka plandaki bazı olaylar var ve biz bunları izlemeliyiz.
Mesela bir iş yapılıyor hemde saati ekranın sağına yazdırmak istiyoruz veya arada diskle ilgili
işlemler yapılınca ne yapacağız.Yada seri portu dinlemek gerekiyor sürekli.Eskiden bunlar zordu ama
artık her işi bir threade havale ediyoruz.Daha kolay

Burada programı hızlandırmak ana temalardan biri.Gui programlamanın olmazsa olmaz bileşeni mesela.

Modern C++ta algoritmaları paralel çalıştırabiliyoruz ve çok kolay. paralel çalışmasını istediğimizde
bir argüman geçiyoruz, onun değerinin ne olduğuna bağlı olarak paralel çalıştırılabiliyor.Başka birşeyle
ilgilenmiyoruz return değeri veya senkronizasyonu ile doğrudan ilgilenmiyoruz.

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

THREAD HEADER FILE
------------------
Threadler lightweight processler. thread denildiğinde std::thread türünden bir nesneden mi
yoksa thread in kendisinden bahsediyoruz. thread in kendisi demek komut zinciri demek ama 
std::thread dediğimde OS teki thread kavramını temsil edecek bir değişkenden söz edeceğiz.

Threadler noncopyable but movable sınıflar.

Thread bir akış zinciri içerisinde yürütülen komut zinciri demek. 

std::thread t; yazıldığında bu sınıf türünden bir nesneden
bahsediliyor ve bu OS teki threadi temsil edebilecek bir değişken.t nesnesi henüz bir thread ile ilişkilendirilmemiş olabilir.

thread header fileda eğer biz bir taski thread halinde çalıştırmak istersek 
thread nesnesi türünden bir nesneye ihtiyaç var. Bu nesneyi default init edebilir.
Buradaki terim işyükü/workload. workload derken bu thread bir kodu çalıştırmak için oluşturuldu.
Standart library de de çalıştırılacak kodun birimi bir function. Bu function herhangibir callable ile 
ilişkilendirilmiş function olabilir. 

Bu threadin çalıştıracağı işyükü olan kod global func olabilir, sınıfın static veya nonstatic func olabilir,
functor sınıfının operator funcı olabilir, lambda ifadesinden elde edilen bir closure type func olabilir.
Yeterki çalıştırılacak bir callable olsun.

Biz bir thread nesnesine bir functionu verdiğimizde o thread nesnesi hayata başladığında (Eğer ctoruna o callable argüman geçilirse)
arka planda OS in bazı funcları çalışacak ve maliyeti var bunların. En çok yapılan hatalardan biride
iş yapılsın diye belirli sayıda thread oluşturup iş hızlansın diye beklemek. Bu threadlerinde
bir maliyeti var ve bunu kurtarmayabilir.

Bir thread bir işyüküne(workload) bağlı olabilir veya olmayabilir.
Default init edilmiş bir thread nesnesinin bir workloadu yok.

int main()
{
	std::thread t;
}

BU DERS BİRAZ KARIŞIK GİDİLECEK !!!
HEPSİ DETAYLI GÖRÜLECEK FUNCLARIN !!!

---------------------------------------------------------------------------------------------------------------------------------------

JOINABLE

Joinable isimli bir func var ve bu function bool döndürüyor.
Eğer bir threadin workloadu varsa, henüz o iş gerçekleştirilmemiş ise yada gerçekleştirilme garantisi
henüz yoksa o thread joinable denilen bir state te.

#include <iostream>
#include <thread>

int main()
{
	std::thread t;

	std::cout << std::boolalpha;
	
	std::cout << t.joinable() << '\n'; // FALSE. DEMEKKİ İŞYÜKÜ YOK.
}


---------------------------------------------------------------------------------------------------------------------------------------

İş yükü veriyoruz.

#include <thread>
#include <iostram>

void func()
{
	std::cout << "func\n";
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main()
{
	std::cout << std::boolalpha;

	std::thread t{func};

	std::cout << t.joinable() << '\n'; // TRUE. WORKLOAD VERİLMİŞ ÇÜNKÜ

	t.join();	// ??? İŞİN YAPILMIŞ OLMA GARANTİSİNİ SAĞLIYOR ????

	std::cout << t.joinable() << '\n'; //  FALSE. ÜZERİNDE BİR WORKLOAD YOK. İŞ YÜRÜTÜLDÜ ÇÜNKÜ.
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------


JOIN
----
Eğer thread nesnesine callable ı bir iş yükü verirsek, thread nesnesinin dtoru çağrıldığında eğer thread
nesnesi halen joinable durumdaysa bu durumda terminate functionu çağrılır !!!!


void func()
{
	std::cout << "func\n";
}

int main()
{
	using namespace std;

	thread t{func}; // t nin hayata gelmesinden itibaren func execute edilmeye başlanıyor.Yani Os in thread için bazı kodlarının 
		 	// çalışması gerekiyor.Tamamen başladı diyemeyiz.Kavramsal olarak çalışıyor diyoruz.
			// Burada 2 thread birden çalışıyor, main thread bizim yarattığımız thread
			// Tabi yarattığımız bitmişte olabilir bunu bilemeyiz.


	t.join(); // joinden sonra artık bu funcın kodu çalıştı ve bitti bundan eminiz !!!!!!!!
		  // Bu noktaya kadar kodun çalışması devam ediyor olabilir ama bu noktadan sonra 
		  // artık beklemez zorunda bunu oluşturan thread
		  // bu kodun çalışması bitecek ve ancak ondan sonra bizim threadimiz çalışmaya devam edecek
}

---------------------------------------------------------------------------------------------------------------------------------------

ÖR:

Senkronizasyon yaptırmadık. Onu daha sonra göreceğiz.Tek bir karakter yadırınca problem değil
ama cümle yazdırsaydık gerekirdi.

#include <thread>
#include <iostream>

void func()
{
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '*';
	}
}

int main()
{
	using namespace std;
	thread t1{func}; 
	
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '.';
	}

	t1.join(); // Joinden sonra artık bu thread altındaki kodları çalıştırmadan önce 
		  	   // bloke olacak ana thread ve burada diğer threadin bitmesi beklenecek.Bu dil garantisi.
			   // for satırında 2 ayrı thread eşzamanlı olarak çalışıyor.

	//...
}

ÇIKTI
-----
***..........................................**.......................**........
...************************.....*********.........................************..
***********......********************************..***..........................
..**.....**..***

Çıktı karışık. Deterministik bir durum değil. Hangisinin yazdırılacağı öngörülemez.

---------------------------------------------------------------------------------------------------------------------------------------

ÖR:

Senkronizasyon yaptırmadık. Onu daha sonra göreceğiz.Tek bir karakter yadırınca problem değil
ama cümle yazdırsaydık gerekirdi.

#include <thread>
#include <iostream>

void func()
{
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '*';
	}
}

void foo()
{
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '?';
	}
}

void bar()
{
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '-';
	}
}

int main()
{
	using namespace std;
	thread t1{func}; 
	thread t2{foo}; // burada foo nun koduda çalıştı  
	thread t3{bar}; // burada barda girdi. main ile birlikte 4 thread var.
	
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '.';
	}

	t1.join();		  	  
	t2.join(); 
	t3.join();
}

ÇIKTI
-----
*--..????------?????--..****----??**..--??**..----****??--....????**......**--???????**
-----**..??--****?????---...******........**---..**??--..**??--......----??..****?????*
*..--????..----??....--??**....**--??..**------????**--....**--??..****??....**??--...

---------------------------------------------------------------------------------------------------------------------------------------

Bir threade bir iş yükü verirsek joinable hale geliyor. Thread nesnesinin Dtoru çağrıldığında
thread nesnesi joinable ise std::terminate çağrılıyor.

#include <thread>
#include <iostream>

void func()
{
	for(int i = 0; i < 1000000; ++i)
	{
		std::cout << '*';
	}
}

int main()
{
	using namespace std;

	set_terminate(my_terminate); // myerminate nutility de var

	thread t{func}; // join edilmemiş bir thread nesnesi yani joinable durumdaki 
					// thread nesnesinin destructoru çağrılırsa terminate çağrılır.
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

Thread sınıfı RAII ile sarmalanmamış. jthread var.

thread nesnesi mutlaka join edilmeli
detach edilmeli. Yani bu benden ayrı çalışsın demek. Eğer joinable olmayan
bir thread join edilirse std::terminate çağrılır.

C++20 de bu eksik kapatıldı. İlerleyen derslerde görülecek.
Thread sınıfını raii ile sarmalayan jthread var. Neden başta böyle yapmadılar?
Bunun farklı sebepleri var. Mesela thread join edilmek zorunda değil, detach te edilebilir.
Bu arka planda çalışsın benim bununla alakam yok demek. Yani bitmesini bekleyip bir threadi bloke 
etmiyoruz. Arka planda o çalışıyor. Farklı farklı problemler kaygılar olabilir burada.

C++ concurrency ile ilgili en önemli uzmanlardan biri Anthony Williams.Concurrency in Action kitabı çok iyi.
JThread yokken kitabında basit bir implementasyonuda vermiş.

joinable bir threadi destroy etmek doğrudan terminate çağrılmasını sağlıyor.
2 ihtimal var.
1- Mutlaka join edilecek
2- Detach edilecek. Detach ayrı bir func.Artık bu benden ayrı çalışsın demek.

---------------------------------------------------------------------------------------------------------------------------------------

Joinable olmayan bir thread nesnesi için join çağrılırsa exception throw eder. !!!

ÖR:
int main()
{
	using namespace std;

	thread tx;	// thread joinable değil.

	try{
		tx.join(); // Exception throw eder.
	}
	catch(const std::exception& ex)
	{
		std::cout << "exception cauhgt : " << ex.what() << '\n';  // HATA
	}
}


ÖR:
int main()
{
	using namespace std;

	thread tx{foo};

	tx.join(); // join edildi. artık joinable değil
	
	try{
		tx.join();  // Yukarıda join çağrıldıktan sonra artık joinable durumda değil.
					// Exception throw eder.
	}
	catch(const std::exception& ex)
	{
		std::cout << "exception cauhgt : " << ex.what() << '\n';  // YİNE HATA
	}
}


---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

BIR IŞYÜKÜNÜ NASIL VERIYORUZ. BU FUNCTIONLARIN ARGÜMANLARI VAR ISE NE YAPILACAK

Argümanlar functiona(yani threadin ctoruna) argüman olarak gönderiliyor.
Arkada variadic bir yapı var.  Threading kendisi template değil ama ctoru template
aldığı argümanları perfect forward ediliyor. Hem callable ın kendisi içn hemde nihai olarak
çağrılacak functionun parametrelerine aktarılması için argümanlar perfect forward ediliyor.

Detach tede sorun oluşabilir. Arka planda sürekli çalışacak, eğer öçalışırken kullandığı değişkenin
ömrü biterse undefined behavior oluşur.

void foo(int x)
{
	std::cout << "foo x = " << x << "\n";
}

void func(int x,int y, int z)
{
	std::cout << "foo x = " << x << "\n";
	std::cout << "foo y = " << y << "\n";
	std::cout << "foo z = " << z << "\n";
}

int main()
{
	using namespace std;

	thread tx{foo,10}; 
	thread ty{func,10,20,30}; 

	tx.join();
	ty.join();
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

REFERANS PARAMETRE

Bazı funcların parametresi referans, yani nesnenin kendisini istiyor.
Burada reference wrapper kullanılacak.


void foo(const std::vector<int>&ivec)
{
	for(const auto val : ivec)
		std::cout << val << " ";
}

int main()
{
	using namespace std;

	std::vector<int> ivec{1,2,3,4,5};
	
	//thread tx{foo,ivec};	// Burada thread sınıfı bizim nesnemizi alıp funca gönderirken sağ taraf değeri olarak gönderiyor !!!!!!!!
				// Dolayısıyla burada funca gelen nesnenin kendisi değil !!!!!!
				// Burada bir kopyalama semantiği var !!!!!!!!
				// const parametre olduğu içinde lvalue referans rvalue expr a bağlandı
	
	thread tx{foo,ref(ivec)};	// Burada ise nesnenin kendisini gönderdik. Bunu reference wrapper ile yaptık.

	tx.join();
}

Eğer thread workload olacak bir functiona, nesnenin kendisini göndereceksek bu durumda reference wrapper
kullanacağız. 

---------------------------------------------------------------------------------------------------------------------------------------

DİKKAT !!!!!!!!!!!!!!!!!!!!!!!

1. CONST OLMAYAN REFERANS PARAMETRESİ VARSA

void foo(std::vector<int>&ivec)
{
	for(const auto val : ivec)
		std::cout << val << " ";
}

int main()
{
	using namespace std;

	std::vector<int> ivec{1,2,3,4,5};

	thread tx{foo,ivec}; // YUKARIDAKİ FUNC PARAMETRESİ CONST DEĞİL. IVEC TE SAĞ TARAF DEĞERİ OLARAK GÖNDERİLİYOR
			     // YUKARIDA SAĞ TARAF DEĞERİ OLARAK GÖNDERİLDİĞİNİ YAZMIŞTIM. BU DURUMDA SOL TARAF REFERANSI
			     // SAĞ TARAF DEĞERİNE BAĞLANAMAZ VE SENTAKS HATASI OLUŞUR.SENTAKS HATASI OLMAMASI İÇİN
			     // FUNC PARAMETRESİ CONST REFERANS OLMALI.
	tx.join();
}



2. FUNC PARAMETRESİ RVALUE REFERANS OLSAYDI

void foo(std::vector<int>&&ivec)
{
	for(const auto val : ivec)
		std::cout << val << " ";
}

int main()
{
	using namespace std;

	std::vector<int> ivec{1,2,3,4,5};

	thread tx{foo,std::move(ivec)}; // Taşıyarak gönderdik.Aslında taşımadak sağ taraf değerine çevirdik.
	
	tx.join();

	std::cout << ivec.size() << '\n';
}


---------------------------------------------------------------------------------------------------------------------------------------

ÖR:

void foo(int& r)
{
	++r;	
}

int main()
{
	using namespace std;

	int x{43};

	// thread tx{foo,x}; Burası sentaks hatası
	thread tx{foo,ref(x)}; Burası geçerli.

	tx.join();

	std::cout << x << '\n';
}

---------------------------------------------------------------------------------------------------------------------------------------

HERTURLU CALLABLE WORKLOAD OLARAK VERİLEBİLİR.

void foo(int x)
{
	std::cout << "x = " << x << "\n";
}

int main()
{
	using namespace std;

	auto fs = foo;

	thread tx{fp,21}; // geçerli

	tx.join();
}

---------------------------------------------------------------------------------------------------------------------------------------

BIR FUNCTOR CLASS OLABILIR

class Functor{
public:
	void operator()(int x)const
	{
		std::cout << "x = " << x << '\n';
	}
};

int main()
{
	using namespace std;

	Functor f;

	thread tx{f,12}; //geçerli

	tx.join();

}

---------------------------------------------------------------------------------------------------------------------------------------

DİKKAT !!!

class Functor{
public:
	void operator()()const
	{
		std::cout << "operator()()\n";
	}
};

int main()
{
	using namespace std;

	thread tx(Functor()); // Burası most mexing parse oluşturur. Hata

	tx.join();
}


MOLA


=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================
=======================================================================================================================================

Thread sınıfının ctoruna bir lvalue expr geçildiğinde buradaki function bunu perfect forward etme işlemi
yapılabilirdi ama özellikle tercih edilmemiş. Standartlar diyorki kopyalama yada taşıma yoluyla aktarılıyor diyor.
Bunun nedenlerinden biri threadin detach edildiğinde dangling referans oluşturma ihtimali.
Ama reference wrapper kullansakta bu risk var dolayısıyla tamamen tercih.

class Myclass
{
public:
	Myclass() = default;
	Myclass(const Myclass&)
	{
		std::cout << "Copy Ctor\n";
	}

	Myclass(Myclass&&)
	{
		std::cout << "Move Ctor\n";
	}
};

void func(const Myclass&)
{
	
}

void foo(Myclass&&)
{
	
}

int main()
{
	using namespace std;

	Myclass m;
	Myclass n;

	thread tx{func,m}; // Copy Ctor çağrıldı
	
	thread ty{func,ref(m)}; // Copy veya Move Ctor çağrılmadı

	thread tz{foo,std::move(m)}; // Move Ctor çağrıldı

	tx.join();
	ty.join();
	tz.join();

}


---------------------------------------------------------------------------------------------------------------------------------------

struct Functor
{
	void operator()()const
	{
		std::cout << "functor::operator()()\n";
	}
};

int main()
{
	using namespace std;

	Functor fx;

	thread t{fx}; // geçerli. operator()() çağrıldığını görürüz

	t.join();

}

---------------------------------------------------------------------------------------------------------------------------------------

LAMBDA IFADESİDE KULLANILABİLİR.

Böylede yazılabilir
int main()
{
	using namespace std;

	thread t{[](int x) { std::cout << x << "\n";}, 10};	// Geçerli

	t.join();
}

---------------------------------------------------------------------------------------------------------------------------------------

SINIFIN MEMBER FUNCINI WORKLOAD OLARAK VERECEĞİZ. 

Burada ctora sınıf nesnesinide argüman olarak geçmek gerekiyor. std::function gibi.

class Nec{
public:
	Nec() = default;
	nec(const Nec& other)
	{
		std::cout << "Copy Ctor\n";
	}
	void func()
	{
		std::cout << "Nec:::func\n";
	}
};

int main()
{
	using namespace std;

	Nec mynec;
	
	thread t{&Nec::func,mynec}; // DİKKAT !!! Geçerli. Burada da copy var
	//thread t{&Nec::func,std::ref(mynec)}; // DİKKAT !!! Geçerli. Burada da copy yok.
	
	t.join();
}

---------------------------------------------------------------------------------------------------------------------------------------


void func(const std::ostream& os);

int main()
{
	using namespace std;

	thread t(func,std::cout); // Sentaks hatası
	thread t(func,ref(std::cout)); // Geçerli
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

THREAD NESNELERİNİN KOPYALANMASI DURUMU
Bu durum mantıklı değil bu sebeple kopyalamaya kapalı ama taşımaya açık. Bir thread joinable durumdaysa ve başka
threade taşınırsa unjoinable duruma geçecek, diğer thread joinable duruma geçecek.Örnek var aşağıda.

void func()
{

}

int main()
{
	thread tx;
	thread ty(tx); //Sentaks hatası. Deleted funca çağrı
	ty = tx; // Sentaks hatası. Deleted olayı yine

	//-------------------------------------------------------------------

	thread tx{func};

	std::cout << boolalpha << tx.joinable() << '\n'; //true

	thread ty{std::move(tx)};

	std::cout << boolalpha << tx.joinable() << '\n'; // false. joinable değil çünkü taşında
	std::cout << boolalpha << ty.joinable() << '\n'; // true. buda taşındığı nesne joinable.
	
	ty.join();
	std::cout << boolalpha << ty.joinable() << '\n'; // false. ty join edildiği için bir üstte artık joinable değil.
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

THREAD NESNESI TAŞINABILIR

void func(std::thread t)
{
	t.join();
}

void workload()
{
	
}

int main()
{
	thread tx{workload};

	func(std::move(tx)); // geçerli

	func(thread{workload}); // Burada funca gönderilen geçici bir thread nesnesi oldu
}

---------------------------------------------------------------------------------------------------------------------------------------

RETURN DEĞERİ THREAD OLABİLİR

std::thread make_thread()
{
	thread t{[]{std::cout << "func()\n";}};

	return t;
}

void workload(){ }

int main()
{
	thread t;

	t = make_thread(); // geçerli.taşıma semantiği devrede çünkü make_thread rvaluexpr.

	t.join();
}

---------------------------------------------------------------------------------------------------------------------------------------

DİKKAT !!!
Hatırlatma

using namespace std;

void func(std::thread&& t)
{
	auto tx = std::move(t); // t burada bir isim ve lvalue expr
				// tkerar rvalue expr ye çevirdik ve taşımayı yaptırdık.
	tx.join();
}

int main()
{
	std::thread tx{[]{}};

	func(std::move(tx)); // Buradaki move sağ taraf değerine dönüştürüyor.
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

JTHREAD

JThreade benzer birşey yazıyoruz

class JThread
{
public:
	JThread()noexcept = default;

	template<typename Func, typename ...Args>
	explicit JThread(Func&& f, Args &&...args) : m_t(std::forward<Func>(f), std::forward<Args>(args)...)
	{
		
	}

	explicit JThread(std::thread t)noexcept : m_t(std::move(t)) { }
	
	JThread(JThread&& other)noexcept : m_t(std::move(other.m_t)) { }

	bool joinable()const noexcept
	{
		return m_t.joinable();
	}

	~JThread()
	{
		if(joinable())
		{
			join();
		}
	}

	void join()
	{
		m_t.join();
	}

	void swap(JThread&other)noexcept
	{
		m_t.swap(other.m_t);
	}

	JThread& operator=(JThread&& other)
	{
		if(joinable())
			join();

		m_t = std::move(other.m_t);

		return *this;
	}

private:
	std::thread m_t;
};

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

GET_ID

Buda bir üye function. Return değeri özel bir tür.Doğrudan tamsayı türü değil.Inserterıda var ama
func return türü get_id isimli bir tür. operator== var ayrıca.

this_thread isimli bir namespace var. Bu namespace te bulunan bazı funclar var. Bu funclardan biride
id yi get ediyor. Farkı o function hangi thread tarafından çalıştırılıyorsa o threadin id sini get ediyor.


#include <iostream>
#include <thread>

using namespace std;

void func()
{
	std::cout << std::this_thread::get_id() << '\n'; // func hangi thread tarafından çalıştırılıyorsa o threadin id si.
}

int main()
{
	thread tx{func};  // 24792. Burada farklı bir thread oluşturulmuş.İd farklı.

	auto id = tx.get_id();

	func(); // 14724. Burası main threadde

	std::cout << typeid(id).name() << '\n'; // class std::thread::id

	std::cout << "thread_id : " << id << '\n'; // 14724. Burasıda main threadde inserter var demiştik.

	tx.join();
}

ÇIKTI
-----
24792
14724
class std::thread::id
thread_id : 14724

AÇIKLAMA!!!!!
Func içerisindeki this_threadin return ettiği id, bu func hangi thread tarafından çalıştırılıyorsa
o threadin id si. Bu bir olanak sağlıyor. Thread id sine göre farklı işlemler yapılabilir. Ana threadden
çağrılırsa bir kod oluşturulan threadden çağrılırsa başka kod çalışsın gibi.


---------------------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <thread>

using namespace std;

std::thread::id g_id;

void func()
{
	auto id = std::this_thread::get_id() << '\n';
	if(id == g_id)
	{
		std::cout << "Func ana thread tarafından calistiriliyor\n";
	}
	else
	{
		std:cout << "Func child thread tarafından çalıştırılıyor\n";
	}
}

int main()
{
	g_id = this_thread::get_id();

	func(); // bu anathread tarafından çalıştırılıyor yazar

	thread tx{func};

	tx.join(); // func child thread tarafından çalıştırılıyor.
}

---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------

SLEEP_FOR & SLEEP_UNTIL

Bunlarda this_thread namespace içinde.Biri çalışan threadi belirli bir süre yada timepointe kadar 
bloke eden funclar.

Sleepfor, bizden bir duration istiyor.
SleepUntil ise o time pointe kadar olan kısmı

void func()
{
	using namespace std::chrono;

	std:this_thread::sleep_for(100ms); // std::chrono::milliseconds{1000} gibi bitşeyde yazılabilirdi.
}

---------------------------------------------------------------------------------------------------------------------------------------

CPPREFERENCE ÖRNEĞİ

#include <iostream>
#include <chrono>
#include <thread>

auto now() { return std::chrono::steady_clock::now(); }

auto awake_time() {
	using std::chrono::operator""ms;
	return now() + 2000ms; // timepoint + duration = timepoint
}

int main()
{
	std::cout << "Hello, waiter...\n" << std::flush;
	
	const auto start {now()};

	std::this_thread::sleep_until(awake_time()); //Bu tarihe kadar yani şimdiki zamanın timepointi + 2000ms sonrasına kadar bloke olacak

	std::chrono::duration<double, std::milli> elapsed {now() - start}; // arada 2saniyeden civarında olmalı. Birazcık daha fazla olabilir.

	std::cout << "Waited " << elapsed.count() << " ms\n";
}

*/
