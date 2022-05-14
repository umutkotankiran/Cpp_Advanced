/*
 
GITHUB NOTLAR
RACE CONDITION: Öyle bir davranış ki bu davranıştan çıkan sonuç iki ya da ikiden fazla thread'in yürütülmesindeki görece sıraya göre değişebiliyor.
Race condition bir sorun oluşturmak zorunda değil. (benign olabilir). Ancak bu terim daha çok problemli durumlar için kullanılıyor. 
(invariant'lar bozuluyor ya da tanımsız davranış oluşuyor.)

DATA RACE: kötü (benign olmayan) bir race condition. data race tanımsız davranış oluşturuyor.
Bir veriyi (data) değiştirmeye (modify) yönelik bir operasyon verinin iki ayrı parçasına erişmek zorunda. 
Bu iki parça ayrı komutlar (instructions) ile değiştiriliyor. Bir thread sadece parçalardan birini değiştirdiğinde (henüz ikinciyi değiştirmeden) 
bir başka thread aynı veriye erişiyor. Bu tür hataları bulmak ya da duplike etmek çok zor olabilir, çünkü çok dar bir zaman aralığında gerçekleşiyor olabilir.
Yazılım geliştirmede "data race" den kaçınmak zorunluluğu karmaşıklığı bir hayli arttırıyor.

DEADLOCK: iki ya da daha fazla sayıda thread ilerleme kaydedemiyor. Her ikisi de diğerinin tuttuğu kaynağı bekleyecek şekilde bloke oluyorlar.

LIVELOCK: iki ya da daha fazla thread (bloke olmadan) birbirlerinin tuttuğu kaynakları beklediklerinden ilerleme kaydedemiyorlar.

CRITICAL SECTION: Birden fazla thread tarafından eş zamanlı olarak erişilmemesi gereken bir kaynak içeren bir kod parçası. 
critical_section girişinden çıkışına kadar, birden fazla thread'in aynı zamanda bu kodları yürütmesini 
engelleyecek şekilde bir senkronizasyon mekanizması oluşturmak gerekiyor.

--------------------------------------------------------------------------------------------------------------------------

Bugünkü konu farklı threadlerin aynı kaynakları/değişkenleri kullanması.
Shared resources.Şimdiye kadar böyle bir problem yok gibi davrandık yada hazır
bazı mekanizmaları gerçekleştirdik ama bunların nasıl yapıldığını konuşmadık.

Multithread programlar singlethread programlara göre çok daha karmaşık.
Bunu oluşturan en temel faktör, bazı değişkenlerin uygulamalarda paylaşımlı olarak kullanılması.
Farklı threadler aynı değişkeni kullanıyor.

Tek bir threadin/main thread çalıştığı programda problem olmayan şeyler, birden
fazla thread oluşturunca öngörülemeyen bazı sorunlar çıkabiliyor.

Problemsiz bir şekilde kaynakların paylaşımlı kullanılmasını sağlayan birden fazla mekanizma var.
Bugün mutex ile ilerleyeceğiz.
Mutual exclusion.

Mutex.h header file içerisinde mutex sınıfları var.

--------------------------------------------------------------------------------------------------------------------------

Problemlere bakalım
Aşağıdaki kodda tek thread de problem yok.Birden fazla thread olursa problem var.

int x;

void f(int);

void func()
{
	//..
	//..

	if(x >= 0) // burada x in doğru kısmına girebilir ama programın akışı aşağıya gelince
	{	 	   // x in değeri negatif bir sayıya başka bir thread tarafından dönüştürülebilir.
		f(x);  // artık burada negatif yada başka bir sayı ile çağrı olabilir.
	}
	else
	{
		f(-x);
	}

}


BURADAKİ PROBLEMLER
-------------------
1 - Unsynchronized data access
	Senkronize edilmemiş veri erişimi.
	Birden fazla thread aynı değişkeni kullanıyorsa, hangi threadin o değişkene daha önce
	erişeceği konusunda garanti yok (Eğer altına alacak mekanizma yoksa) 
		
	Race Condition : Eğer işlemlerin sonucu bir şekilde birden fazla threadden hangisinin
	önce paylaşılan varlığa erişeceğine bağlı olarak değişiyorsa böyle durumlara race condition deniyor.
	Data race değil. 
	
	Race condition bir hata olmak zorunda değil.hangi threadin kaynağa ilk ulaşacağına bağlı olarak
	farklı sonuçlar oluşturabilir ama programın genel mantığı açısından bir problem olmayabilir.
	Yani race condition zararlı olmayabilir.

	Data race dendiğinde bu zararlı bir race condition.
	C++ standartlarına göre data race undefined behavior.

	Threadlerden biri yazma amaçlıysa diğerinin yazma yada okuma amaçlı olmasının önemi yok
	her zaman data race riski var.

2 - Half-written data
	Bir thread değişkeni değiştirdi, diğer thread onun değişmiş halini okuyor. Tam değiştirilme sırasında
	ara bir durumda ona erişmesi sözkonusu değil. 4-8 bytelık bir değişkenimiz ilgili sistemde
	işlemci komutlarıyla 2 aşamada değiştiriliyor olabilir. Burada bir thread yazdığında henüz bir kısmı
	değiştirişmişken, 2. store işlemi yapılmamışken diğer thread onu okursa çok farklı bir değer oluşabilir.
	Bu undefined behavior.

3 - Reordered Statements 
	Derleyici optimizasyonlar yapıyor. Konuşmuştuk C de.
	İşlemlerin sırasını değiştirmekte buna dahil. Single thread çalışınca derleyici buna uygun reordering
	yapılıyor. Gözlenebilir değişiklik yok gibi geliyor ama birden fazla thread çalışınca reordering
	tamamen işimizi bozabilir.
	
	Not : Link zamanında yapılan optimizasyonlar önemli.

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

HATALI KOD ÖRNEKLERİ
--------------------

1. Bu örnekte değişken paylaşımlı olarak kullanılabilir.
   Multithread çalıştırıldığında vector doluyken if in doğru kısmına girebilir.
   ama dvec.back() çalışmadan vector boş hale gelebilir. Undefined behavior olabilir.
   STL containerları multithread doğru çalışma garantisi vermiyor.

include <vector>
include <iostream>

std::vector<double> dvec;

void func()
{
	//...

	if(!dvec.empty())
	{
		std::cout << dvec.back() << std::endl;
	}

	//...
}

------------------------------------------------------------------------------------------------------

2. Burada yine problem var. Birden fazla thread çalıştığında bir senkronizasyon olmadığı için
   g, 0 veya 1 olarak kullanılır diyemeyiz. Bunun atomik olma garantisi olsaydı bunu söyleyebilirdik.

#include <cstdint>
#include <iostream>

std::int64_t g{};

void foo()
{
	//...
	g = -1;
}

void bar()
{
	std::cout << g << std::endl;
}

Komutlar o kadar hızlı yürütülüyorki bazı hataları gözlemleyemeyebiliriz. Zaman çok kısa çünkü.
Bu yüzden hata olduğu halde hata vermeyipte programcıyı yanıltan durumlar olabilir.

------------------------------------------------------------------------------------------------------

3. g ve ready_flag shared variable. bar functa ready_flag false olduğu sürece dönüyor.
	foo içinde de ready flag ve g değeri değiştiriliyor. Functionların çalışma sırasına göre 
	functionların davranışları değişiyor.

#include <cstdint>
#include <iostream>

std::int64_t g{};
bool ready_flag{};

void foo(std::int64_t x)
{
	//...
	g = x;
	ready_flag = true;
	//...
}

void bar()
{
	//...
	while(!ready_flag)
	{
		//...
	}

	foo(g);
}

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

ÇOK ÇOK ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!

Burada bir problem olmasını istemiyorsak paylaşılan verinin kullanıldığı kod alanı ki buna critical section deniyor.
Burayı koruma altına almamız gerekiyor.Yani paylaşılan değişkeni 2 thread paralel olarak kullanmayacak.
Önce biri ona erişecek onun erişimi tamamlandıktan sonra diğeri erişecek. Bunu sağlayan en primitive
mekanizma mutex(mutual exclusion).

Klasik örnek
Tuvalete 1 kişi girebilir. Tuvaletinde anahtarı var. Tuvalete giren anahtarını alıyor ve kapıyı kitliyor.
Gelen 2. kişi beklemek zorunda. İlk giren işini bitirip kilidi açacak anahtarı bırakacak.
Öbürü de anahtarı alıp kendi kitleyecek .... diye gidiyor.

Mutex te böyle, bir kod alanına giren thread eğer orası mutual exclusion ile koruma altına alınmışsa öncelikle
kilidi edinmek zorunda.

Burada bazıları mutexi edinmek(acquire) kullanıyor. Bazıları mutexi kilitlemek(lock) diyor.
Aynı anlamda bunlar. 

Threadlerden biri mutexi kilitleyince, oraya gelen başka bir thread orada beklemek zorunda.
Beklemek derken kastedilen bloke olmak.Ancak o kilit serbest bırakıldığında yani unlock/release edildiğinde
öbürü kilidi edinebilir. Kilit sadece 1 thread tarafından edinilebilir. Diğeri kilidi unlock ettiğinde
diğeri edinebilir. Mutex öyle bir değişkenki 2 state den birinde olabilir. Ya edinilmiştir bu durumda
onu edinmek için orada bloke olacağız yada edinilmemiş onu edinebiliriz. Mutexi acquire eden/ edinen
belirli bir noktada onu serbest bırakıyor. 

NOT : Bir mutexin lock 2 kere çağırmak ya undefined behavior yada UB oluşturur.

mtx.lock();

	// critical section

mtx.unlock();

------------------------------------------------------------------------------------------------------

KARIŞTIRILAN KONU
std::mutex bir mutex sınıfı. kritik kod alanını korumaya alacaksak bir mutex nesnesini kullanacağız.
Bu mutex nesnesi birçok durumda birden fazla threadin erişebileceği bir durumda olmalı. Global değişken
yada sınıfın veri elemanı olabilir.

Farklı mutex sınıflarıda var. recursive mutex, timed mutex, recursive time mutex isimli sınıflar var.
Özünde hepsi mutex sınıfı ama hangi mutexi kullandığımıza bağlı olarak ilave işlevsellikleride var.

MUTEX SINIFLARI
1 - std::mutex
2 - std::timed_mutex
3 - std::recursive_mutex
4 - std::recursive_timed_mutex


Burada minimal hangisi işimizi görürse onu kullanmalıyız. Muhtemelen diğerlerinin ilave bir maliyeti var.
Mesela recursive olanlar.

Bunlar sonuçta lock ve unlock funclarına sahipler.
Kilidi edinmek için lock, serbest bırakmak için unlock funcını çağırmak gerekiyor.
Bu durumlarda her zaman devreye RAII devreye giriyor.
Mutexi lock ettikten sonra unlock etmeyi unutursak burası problem. Ya deadlock oluşacak yada
duruma göre UB olacak yada derleyicinin bazı öngördüğü durumlarda exception throw etme hakkıda var.

Burada 1. problem mutexin kilitlenmesinin açılması unutulmamalı.
Lock ettikten sonra unlock etmeliyim ki başka thread onu bıraksın

Birde ortak başka bir problem exception. Unlock ederiz ama unlock çağrısının yapıldığı noktaya gelene kadar
bir exception throw edilirse unlock yapılmadan programın akışı hander koda geçer. Bu tip durumlarda
RAII sınıfları devreye giriyor. Mutex sarmalayan sınıflar. Mutexi doğrudan kullanmak yerine ctoru ile mutexi edinen
ama dtoru ile wrap ettiği mutexi unlock eden sınıflar kullanılıyor. Bunlar işmizi kolaylaştırıyor.

Bu sınıflarla mutex sınıflarını karıştırmamak gerekiyor.
Bir tanesi lock guard sınıfı.


------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

MUTEX SARMALAYAN RAII SINIFLARI
-------------------------------
1 - lock_guard
2 - unique_lock
3 - scoped_lock  C++17
4 - shared_lock

mutex sınıfları yukarıdaki sınıf şablonlarında argüman olarak kullanılıyor.
ör: 
lock_guard<mutex>
lock_guard<timed_mutex>
scoped_lock<mutex> ... gibi

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
LOCK GUARD
----------
std::lock_guard
En temel raii sınıfı bu. Kısıtlı olanaklar sağlıyor. Daha geniş olanaklar sağlayan raii sınıfları da var.
Sınıfın kurcu işlevi mutex'i kilitliyor. Sınıfın sonlandırıcı işlevi kilidi açıyor.
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
UNIQUE LOCK
-----------
Bu daha geniş olanak sağlayan bir sınıf şablonu. Bu sınıfların hepsi sınıf şablonu.
Lock guarddan farkı kilidi edinme konusunda farklı stratejileri var.
Zaten edinilmiş kilidi sarmalayabilir, yada kilidi daha sonra edinmek üzerine sarmalayabilir.
Olabilecek en kabiliyetlisi.

Locks a mutex in exclusive mode. The constructor also accepts arguments that instruct it to timeout instead of blocking forever on the lock call. 
It is also possible to not lock the mutex at all, or to assume that it is locked already, or to only try locking the mutex. 
Additional methods allow to lock and unlock the mutex during the unique_lock lock’s lifetime.


template <class Mutex>
class unique_lock;
std::unique_lock genel amaçlı bir mutex sarmalayıcısı. mutex'in edinilmesi için farklı stratejiler sunuyor:

Özellikler
- Deferred locking (sınıfın kurucu işlevi ile kilidi edinebildiğimiz gibi daha sonra nesnenin lock işlevini de çağırabiliyoruz.)
- Belirli süreyle sınırlandırılmış kilitleme girişimi olanağı,
- Birden fazla kez kilitleme
- Kilit mülkiyetinin transferi
- Condition variables ile kullanılma olanağı

Ayrıca,
- std::unique_lock nesneleri kopyalanamıyor ancak taşınabiliyor. std::lock_guard sınıf nesnelerinin taşınamadığını hatırlayalım.
- std::lock_guard ile aynı arayüze sahip ama daha fazla olanak sağlıyor. Kilitlemenin ne zaman ve nasıl olacağını belirleyebiliyoruz.

Avantajı,
Bu sınıfın temel avantajı şu: std::unique_lock nesnesinin destructor'ı çağrıldığında kilit edinilmiş durumda ise kilidi serbest bırakır,
kilit edinilmiş durumda değil ise destructor bir şey yapmaz.

sınıfın kurucu işlevine argüman olarak try_to_lock geçersek kurucu işlev kilidi edinmeye çalışır ama thread'i bloke etmez.

std::mutex mtx;

void func()
{
    std::unique_lock<std::mutex> guard(mutex, std::try_to_lock);
    if (guard) { //kilit edinilmis ise

    }
}

mutex türü olarak std::time_mutex kullanarak belirli süre bir kilidi edinmeye çalışmasını sağlayabiliyoruz:

std::mutex mtx;

void func()
{
    std::unique_lock<std::mutex> guard(mtx, std::try_to_lock);
    if (guard) { //kilit edinilmis ise

    }
}
std::time_mutex kullanarak kurucu işleve bir std::duration geçebiliriz:

std::unique_lock<std::timed_mutex> lock(mutex, std::chrono::seconds(1));
sınıfın kurucu işlevine argüman olarak std::defer_lock geçilirse mutex'i edinmez. daha sonra sınıfın lock fonksiyonlarından birini çağırmamız gerekiyor.
sınıfın kurucu işlevine std::adopt_lock geçilirse ilgili thread'in zaten bu mutex'i edindiği varsayılır.
edinilen mutex'in sahipliğini başka bir nesneye aktarabiliyoruz.

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SCOPED LOCK
-----------
std::scoped_lock
scoped_lock birden fazla mutex'in deadlock olmadan edinilmesini sağlıyor.
std::lock_guard sınıfına benziyor. Ancak sınıfın kurucu işlevi birden fazla mutex nesnesini alabiliyor. 
Kurucu işlev aldığı sırayla mutex nesnelerini kilitliyor. Sonlandırıcı işlev ters sırada kilitleri açıyor.
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SHARED LOCK
-----------
Okuma amaçlı erişim tarafından birden fazla thread okuma amaçlı erişim sağlayabiliyor
Yani, bir thread paylaşılan kaynağa yazma amaçlı erişirken birden fazla thread okuma maçlı erişebiliyor
shared_lock reader/writer lock
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

Aşağıdakiler ise mutex sınıfları zaten yukarıda yazmıştık.
1 - std::mutex
2 - std::timed_mutex
3 - std::recursive_mutex
4 - std::recursive_timed_mutex

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

C++17 ye kadar en minimal raii sınıfı lock_guard idi.
unique lock ihtiyacı yoksa yada birden fazla threadin okuma amaçlı aynı kilidi edinmesi gibi tema yoksa
lock_guard kullanılıyordu.

C++17 ile eleştirenler olsada lock_guard yerine scoped_lock standart hale geldi.Çünkü scoped lock ın
fazlası var ve eksiği yok. Analizlere göre maliyet açısından dezavantajı yok. C++17 öncesi RAII hepler
olarak lock_guard varken artık scoped_lock var.

Neden birden fazla mutex sınıfı var ?
Çünkü birden fazla mutex sınıfı farklı interfacelere sahip.

std::mutex in lock, unlock ve try_lock funcını çağırıyoruz. ama kilidi 200ms boyunca edinmeye çalış.
edinebilirsen edin edinemezse edinme, gibi bir işlevi yok. 
Zamana bağlı kilidin edinilme girişşimi yapabilmek için timed_mutex yapılması gerekiyor.

Recursive olması, normalde bir mutexi lock edince onu tekrar lock edemeyiz, edersek derleyici anlayabilirse
bir exception throw edebilir yada undefined behavior oluşur.
Mesela bir func için bir critical sectionu bir mutex ile koruma altına almak için mutexin lock funcını çağırdık
çıkıştada unlock çağırdık fakat o arada da sınıfın başka bir member funcını çağırdık. Aynı mutex
ile orayıda korumak istiyoruz, birdaha lock edersek exception veya UB oluşur.
Yani std::mutex birden çok kere lock edilemiyor.Burada thread ile işlem yapsakdık, thread kilidi bekleyecekti.
Diğeri release ettiğinde bu sefer bekleyen thread kilidi acquire edecek.

Recursive mutex ise n kere lock edilebilir. n değeri derleyiciye bağlı.
Recursive ile timed arasındaki fark bu.
Recursive ve timed değilse 1 kere kitleyebiliriz ve belirli bir süre sonra kilidi elde etme girişimi yapamayız.

------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <mutex>
#include <chrono>

std::mutex m1;
std::timed_mutex m2;
std::recursive_mutex m3;
std::recursive_timed_mutex m4;


int main()
{
	using namespace std;
	using namespace std::literals;

	m1.lock();   // Ya kilidi edinir yada bloke olur.Bu mutex başka bir thread tarafından tutuluyorsa,
		     // diğer thread bu koda geldiğinde, ilk thread o mutexi bırakana kadar lock ı çağıran thread
		     // bloke olacak.Ya alacak yada alamaz. Alırsa yoluna devam eder alamazsa bloke olur.

	m1.try_lock();  // Boolean döndürüyor.Kilidi elde etmeye çalışıyoruz. Kilit boştaysa kilidi elde ederiz ve true döndürüyor.
		       // Kilit boşta değilse bloke olmuyor ve false döndürüyor.

	m1.unlock();  // Kilidi bırakıyor

	------------------------------------------------------------------------------------------

	m2.lock();

	m2.try_lock_for(1000ms); // duration ister. 1000ms boyunca kilidi edinmeye çalışır.Boolean döndürür.
				 // 1000ms boyunca kilidi elde etmeye çalışır ederse true dönder, elde edemezse false döner.
	
 	m2.try_lock_until(std::steady_clock::now() + 1000ms); // timepoint ister. O time pointe kadar dener. Gerisi yukarısı ile aynı.
	 
	m2.unlock();

	------------------------------------------------------------------------------------------

	m3 ve m4 te daha önce bahsedildi yukarıda. Örnekleri yapılacak

}

------------------------------------------------------------------------------------------------------

ÖR:

#include <thread>
#include <mutex>
#include <iostream>

unsigned long long counter = 0;
//std::mutex mtx;  // DİKKAT!!!! YORUM SATIRI ŞUANDA

void func()
{
	//mtx.lock(); // DİKKAT!!!! YORUM SATIRI ŞUANDA

	for (unsigned long long i = 0; i < 1'000'000ull; ++i) {
		++counter;
	}
	
	//mtx.unlock(); // DİKKAT!!!! YORUM SATIRI ŞUANDA
}

int main()
{
	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);
	std::thread t4(func);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << counter << '\n';
}


Single thread çalışsa counter değeri 1milyon olur. 4 thread oluşturduk ve workload olarak func verildi
sonra hepsi join edildi ve değerine bakıldı.Burada write işleminin düzgün yapılma şansı yok.
Biri write işlemi yaparken öbürüde aynı değere yine write yapabilir:D::D:D:D
Şaka gibi burası. 4'000'000 beklerken 1'155'942 çıktı. Release modda da 1'000'000 :D::D::D:D::D:D

NOT : Kritik kod alanı/critical section ne kadar geniş olursa maaliyet o kadar yüksek olur.

YORUM SATIRLARINI KODA DAHİL EDİNCE.
4'000'000 çıktı değer.

Burada t1, t2 t3 t4 ün çalışma sırası hakkında garanti yok tabi.

------------------------------------------------------------------------------------------------------

NOT: Mutexi unlock etmezsek. Ub yada deadlock olabilir yada exception verebilir.

void func()
{
	mtx.lock();

	for (unsigned long long i = 0; i < 1'000'000ull; ++i) {
		++counter;
	}
	// mutexi unlock etmedik. 
}

Burada ya deadlock oluşacak yada undefined behavior olacak.
Birde live lock var. Deadlockta bloke olmuş durumdalar
Live lockta bloke olmadan diğerlerinin kaynağını bekliyorlar. Bloke yok.

------------------------------------------------------------------------------------------------------

Biz yukarıdaki örnekte RAII sınıflarından birini kullanabiliriz.
lock_guard kullanalım

#include <thread>
#include <mutex>
#include <iostream>

unsigned long long counter = 0;
std::mutex mtx;

void func()
{
	std::lock_guard<std::mutex> guard(mtx); // DİKKAT !!! 
						// mutexi edinecek.Tabi edinene kadar bloke olacak.
						// Scope u bittiğinde lock_guard dtoru sarmaladığı mutexin
						// unlock funcını çağıracak.

	for (unsigned long long i = 0; i < 1'000'000ull; ++i) {
		++counter;
	}
}

int main()
{
	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);
	std::thread t4(func);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << counter << '\n';
}


------------------------------------------------------------------------------------------------------

ÖR: Mutexi zaten edinmişte olabiliriz.Lock etmeme rağmen dtorun bunu unlock etmesinden faydalanmmak istiyoruz.

#include <thread>
#include <mutex>
#include <iostream>

unsigned long long counter = 0;
std::mutex mtx;

void func()
{
	mtx.lock(); // lock etmeme rağmen dtorun bunu unlock etmesinden faydalanmmak istiyoruz.
				
	// Bunun için farklı bir ctor var.
	// Artık ctor bunu tekrar edinmeye çalışmıyor.
	std::lock_guard<std::mutex> lg(mtx,std::adopt_lock); 
	
	for (unsigned long long i = 0; i < 1'000'000ull; ++i) {
		++counter;
	}
}

int main()
{
	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);
	std::thread t4(func);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << counter << '\n';
}

------------------------------------------------------------------------------------------------------

DAHA KABİLİYETLİ SINIF UNİQUE LOCK

Lock guarddan farkı kilidi edinme konusunda farklı stratejileri var.
Zaten edinilmiş kilidi sarmalayabilir, yada kilidi daha sonra edinmek üzerine sarmalayabilir.
Olabilecek en kabiliyetlisi. Zaten yukarıda açıklamalarını yazmıştım.

------------------------------------------------------------------------------------------------------

ÖRNEKLERE DEVAM

#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>

std::mutex mtx;

void print_block(int n, char c)
{
	using namespace std::literals;

	//mtx.lock();
	// std::lock_guard guard{mtx}; // Bu şekilde de yapabilirdik.CTAD var yine.
	for (int i = 0; i < n; ++i) {
		std::this_thread::sleep_for(5ms);
		std::cout << c;
	}
	std::cout << '\n';

	//mtx.unlock();
}

int main()
{
	std::thread th1(print_block, 50, '*');
	std::thread th2(print_block, 50, '$');
	std::thread th3(print_block, 50, '+');
	std::thread th4(print_block, 50, '!');

	th1.join();
	th2.join();
	th3.join();
	th4.join();
}

SENKRONIZE EDILMEDEN(YOrum satırları dahil edilmeden)
!$*+!*$+$+*!*!+$$!+**+!$$+*!$+*!!*+$$*+!*!+$+$!*!*$++$*!+!*$$*!++!*$$*!++!$**!$+$*!++!$*$*+!!$+*+*$!*+$!!$+**+!$+*!$$!*+!+$**$+!!+$**$!++$!*!+*$*$!+!+*$$*!+$*!+!+$**$+!$*+!+*!$*+!$!$+*$!*+$!*+!$+*+
!
$
*

SENKRONIZE EDİLEREK(Yorum satırlarını koda dahil ederek)
**************************************************
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
++++++++++++++++++++++++++++++++++++++++++++++++++
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

------------------------------------------------------------------------------------------------------

ÖR:

#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>

int gcounter{};
std::timed_mutex mtx;

void increment(int i)
{
	using namespace std::literals;


	if (mtx.try_lock_for(50ms)) {  // try lock true veya false döner ve bloke etmez.
		++gcounter;
		std::this_thread::sleep_for(10ms);
		std::cout << "thread : " << i << " kritik bolgeye girdi\n";
		mtx.unlock();
	}
	else
		std::cout << "thread  " << i << " kritik bolgeye giremedi\n";
}

int main()
{
	std::thread t1{ increment, 1 };
	std::thread t2{ increment, 2 };

	t1.join();
	t2.join();
	std::cout << "gcounter = " << gcounter << "\n";
}

------------------------------------------------------------------------------------------------------

mutex::try_lock ornek kod
mutex sınıflarının try_lock fonksiyonları mutex'i edinemezse ilgili thread'i bloke etmiyor.
fonksiyon bu durumda false değer dönüyor.
böylece kilidi edinemediğinde / kilidi edinene kadar başka işlemler yapabiliyoruz.
Kilidi edinmiş bir thread'in tekrar try_lock fonksiyonunu çağırması tanımsız davranış.
eğer aynı thread'in aynı mutex'i birden fazla kez kilitlemesi gerekiyor ise std::recursive_mutex kullanılmalı


#include <iostream>
#include <thread>
#include <mutex>

int counter{};					// atomik olmayan paylaşılan değişken.
std::mutex counter_mtx;			// counter değişkenine erişim sağlayacak mutex.Senkronizzsyona yönelik.

void try_increase()
{
	for (int i = 0; i < 100'000; ++i) {
		if (counter_mtx.try_lock()) {   // Sadece mutex edinilirse counter arttırılacaks
										// try lock mutex boştaysa edinir ve true döner boşta değilse edinemez false döner
			++counter;
			counter_mtx.unlock();
		}
	}
}

int main()
{
	std::thread ar_t[10];  // thread dizisi

	for (int i = 0; i < 10; ++i)
		ar_t[i] = std::thread(try_increase); // dizi elemanlarına değerleri veriliyor.

	for (auto& t : ar_t)
		t.join();

	std::cout << counter << " kez arttirma islemi yapilabildi.\n";
}

10 * 100000 den 1000000 olmayacak çünkü döngünün bazı turlarında lockı acquire edecek bazılarında edemeyecek.
108027 gibi bir sonuç çıktı.Yani bu kadar kez kilit/lock edinilmiş.

------------------------------------------------------------------------------------------------------

#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
#include <syncstream>

int gcounter{};
std::timed_mutex mtx; // bu sefer timed mutex. 

void increment(int i)
{
	using namespace std::literals;

	//süreyi 5ms yap yine dene
	if (mtx.try_lock_for(50ms))  // 50ms boyunca kilidi elde ederse true döner, edemezse false
	{
		++gcounter;		// sayacı 1 artırdı

		std::this_thread::sleep_for(10ms);  // gecikme verdi 
		
		std::osyncstream{std::cout}  << "thread : " << i << " kritik bolgeye girdi\n";
		
		mtx.unlock(); // unlock edildi.
	}
	else
		std::osyncstream{std::cout} << "thread  " << i << " kritik bolgeye giremedi\n";  // kilidi 50ms içinde elde edemezse bnuraya girer.
}

int main()
{
	std::thread t1{ increment, 1 };
	std::thread t2{ increment, 2 };

	t1.join();
	t2.join();
	std::cout << "gcounter = " << gcounter << "\n";
}

OUTPUT
------
thread : 1 kritik bolgeye girdi
thread : 2 kritik bolgeye girdi
gcounter = 2

------------------------------------------------------------------------------------------------------

Standart outputa yazmak sözkonusu olduğunda stdio ile senkronize edilmişse şu garanti var.
Karakter bazında herhangibir problem yok ama karanterler arasında problem olabilir.

#include <sstream>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>


struct pcout : std::ostringstream {  // DİKKAT!!!  Amaç pcout kullanınca yazma işleminin senkronize edilmiş
public:								// bir şekilde kullanmasını sağlıyoruz.

	~pcout()
	{
		std::lock_guard locker{ cmutex }; //CTAD
		
		std::cout << rdbuf(); // rdbuf, streambuf * döndürüyordu.Temel C++. ostream inde streambuf * parametreli bir inserterı var.
				      // Bu inserter bufferdaki tüm karakterleri akıma veriyor.Hatta bir dosyayı yazdırmanın en kısa yolu demiştik Temel C++ta.
				      // Burada nesne destruct olduğunda rdbuf funcı çağrılacak bufferdakileri çıkış akımına verecek.
				      // lock guard kullandıkki birden fazla thread giremesin buraya.
		std::cout.flush();
	}
	static inline std::mutex cmutex;
};

void print(int x)
{
	pcout{} << "print islevi x = " << x << "\n";
	//std::cout << "print islevi x = " << x << "\n"; // burada çıkış karmaşık oldu.

}

int main()
{
	std::vector<std::thread> tvec;

	for (int i = 0; i < 20; ++i)
		tvec.emplace_back(std::thread{ print, i });

	for (auto& t : tvec)
		t.join();
}

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

BİRDEN FAZLA LOCK YAPARSAK NE OLUR


#include <mutex>
#include <iostream>
#include <exception>

std::mutex mtx;

int main()
{
	try {
		mtx.lock();
		mtx.lock(); // Burada derleyici exception gönderebilir ama garantisi yok.Derleyici her zaman anlamayabilir.
					// exception gönderirse system error türünden exception verir.
	//...
	}
	catch (const std::exception& ex) {
		std::cout << "exception caught: " << ex.what() << '\n';
		//exception caught: device or resource busy: device or resource busy
	}
}

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

Burada problemlerden biri bir mutexin birden fazla kez kilitlenme talebi.
Bunu normal mutex ile değil recursive mutex ile yapabiliriz.
Bu örnekleri raii sınıflarıyla kullanıyoruz normalde. Bu örnekler nasıl çalıştığını gösteriyor.

#include <mutex>
#include <iostream>
#include <thread>


//std::mutex rmtx; Bunuda dene.
std::recursive_mutex rmtx;

int gcount = 0;

void rfunc(char c, int n)
{
	if (n < 0)
		return;

	rmtx.lock();  // Recursive mutex olmazsa bu ayı kilidi tekrar kitlediğimiz için runtime da problem olacak.
	std::cout << c << ' ' << gcount++ << '\n';
	rfunc(c, n - 1); 
	rmtx.unlock();
}

int main()
{
	std::thread tx{ rfunc, 'x', 10 };
	std::thread ty{ rfunc, 'y', 10 };

	tx.join();
	ty.join();
}

------------------------------------------------------------------------------------------------------

#include <mutex>
#include <iostream>

class Nec {
public:
	void func()
	{
		std::lock_guard<std::mutex> guard{mtx};
		std::cout << "func cagrildi" << std::endl;
		foo();
		std::cout << "func sona eriyor" << std::endl;
	}

	void foo()
	{
		std::lock_guard<std::mutex> guard{mtx};	// Buda functaki ile aynı mutexi kullanıyor. !!!!!

		std::cout << "foo cagrildi" << std::endl;
		std::cout << "foo sona eriyor" << std::endl;
	}

private:
	std::mutex mtx; // sınıfın birden fazla member funcında aynı mutexi kullanmanın en tipik yolu mutexi
					// sınıfın veri elemanı yapmak.
					// Burada funclar const member func olsaydı mutex nesnesinin lock functionu çağrıulsaydı
					// sentaks hatası olurdu. Burada veri elemanı mutable olmalıydı o zaman !!!!!!!!!!!!!!!!!
};

int main()
{
	Nec nec;

	try {
		nec.func(); //may throw exception
	}
	catch (const std::system_error &ex) {
		std::cout << "exception caught: " << ex.what() << '\n';
	}
}


Func mutexi kilitledi. O da foo yu çağırdı ve aynı mutex tekrar kilitlenmeye çalıştı.
Bu durumda recursive mutex değilse hata oluşacak. EXCEPTİON VEREBİLİR !!!!!
AMA GÖNDERMEYEDEBİLİR!!!!!!
DERLEYİCİYE BAĞLI.

------------------------------------------------------------------------------------------------------

Mutexi recursive mutex yapalım.

#include <mutex>
#include <iostream>

class Nec {
public:
	void func()
	{
		std::lock_guard<std::recursive_mutex> guard{mtx};
		std::cout << "func cagrildi" << std::endl;
		foo();
		std::cout << "func sona eriyor" << std::endl;
	}

	void foo()
	{
		std::lock_guard<std::recursive_mutex> guard{mtx}; // artık recursive mutex var. tekrar lock edilebilir.
		std::cout << "foo cagrildi" << std::endl;
		std::cout << "foo sona eriyor" << std::endl;
	}

private:
	std::recursive_mutex mtx;
};

int main()
{
	Nec nec;

	try {
		nec.func(); //Artık hata yok. Recursive mutex kullandık.
	}
	catch (const std::system_error &ex) {
		std::cout << "exception caught: " << ex.what() << '\n';
	}
}

------------------------------------------------------------------------------------------------------

Mülakatlarda sorulan sorulardan biri
Kodda problem var mı?

#include <mutex>
#include <iostream>

int x{};
std::mutex mtx_func;
std::mutex mtx_foo;

void func()
{
	std::lock_guard guard{mtx_func};
	for(int i = 0; i < 1000;++i)
	{
		++x;
	}
}

void foo()
{
	std::lock_guard guard{mtx_foo};
	for(int i = 0; i < 1000;++i)
	{
		++x;
	}
}

int main()
{
	std::thread t1{func};
	std::thread t2{foo};

	t1.join();
	t2.join();
}

X in değerinin 2000 olma garantisi yok
Kod problemli.

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

UNIQUE LOCK
-----------
std::unique_lock<std::mutex> ulock(mtx); // Kilidi edinene kadar bloke olacak

std::unique_lock<std::mutex> ulock(mtx, std::adopt_lock); // Sadece sarmala anlamında çünkü zaten mutex acquire edilmiştir. Yani yukarıda mtx.lock() zaten var 

std::unique_lock<std::mutex> ulock(mtx, std::defer_lock);  // kilidi elde etmek istemezsek daha sonra istersek diye kullanıyırouz

std::unique_lock<std::mutex> ulock(mtx, std::try_to_lock); // Kilidi edinmeye çalışacak edinirse operator bool true döner, edinemezse false döner
							   // operator bool yerine owns_lock() funcıda kilidi edinip edinemediğini sorgular.


#include <iostream>
#include <mutex>

std::mutex mtx;

//Aşağıdaki gibi olabilirdi.
void func()
{
	mtx.lock(); // Burada kilitleyip
	std::unique_lock<std::mutex> lock(mtx,std::adopt_lock); // lock guarda gönderilebilir.
}

//Bu şekilde de olabilir
void func()
{
	std::unique_lock<std::mutex> ulock(mtx,std::defer_lock); // kitlemeye çalışma anlamında std::defer_lock
								// kilidi edinmek için lock funcını çağırmalıyız

	// ulock.try_lock ... funclarını çağırabiliriz.try_lock for veya untili çağırabilmek için edinilen mutexin
	// std::timed_mutex olması gerekiyor.
	
	ulock.lock();
}


//Bu şekilde de olabilir
void func()
{
	std::unique_lock<std::mutex> ulock(mtx,std::try_to_lock); 
}


Yani Ctora 3 farklı parametre geçilebilir.
std::adopt_lock
std::defer_lock
std::try_to_lock


------------------------------------------------------------------------------------------------------

Mutex timed mutex ise ctoruna duration da geçebiliyoruz.

#include <mutex>
#include <iostream>


std::timed_mutex mtx;

void func()
{
	using namespace std::literals;

	std::unique_lock<std::timed_mutex> ulock(mtx, 500ms); // ya 500ms içinde kilidi edinecek yada edinememiş olacak.
	
}

Burada sınıfın destructoru, eğer kilit edinilmiş durumdaysa unique_lock ın destructoru çağrıldığında 
kilidi efectively unlock ediyor ama zaten kilit edinilmemiş durumdaysa burada tanımsız davranış yok.

Edinilmemiş durumdaysa biz lock çağırsak normalde undefined behavior olur.

belirli yerlerde kilidin mülkiyetinin transfer edilmesi gerekiyor.

------------------------------------------------------------------------------------------------------

Edinilmiş kilidin mülkiyetinin aynı sınıf türünden başka nesneye transferi için sınfın moveable olmalı. 
lock guard sınıfın non copyable ve non movable 
unique lock sınıfı non copyable but movable

2:13
Burada unique_lock isimli github notlarına baktık.

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

STD::LOCK
---------
Bu global bir function. Bazı durumlarda bizim birden fazla mutexi elde etmemiz gerekiyor.
Ör, 2 tane shared variable var. Bu shared variable ları farklı mutexler ile senkron etmek gerekiyor.
Dolayısı ile aynı kod içinde 2 ayrı nesne kullanıldığı için biz 2 tane mutexi birden edinmek zorunda kalıyoruz.

Bu dikkat edilmezse problemli bir durum. Çünkü aynı sırada kilitleri edinip serbest bırakmazsa farklı threadler,
tipik deadlock senaryosu oluşuyor. Deadlock senaryosu, threadlerden biri kilidi edindi, diğeride diğer
kilidi edindi. Şimdi ikiside 2. kilidi edinmek için kilidin serbest bırakılmasını bekliyor ama 1 thread
1 tanesini diğeri ise diğerini tutuyor, bu durumda ikiside ilerleyemiyor. Bloke olmuş olarak kalıyor.

------------------------------------------------------------------------------------------------------
GİTHUB NOTLARI

std::lock fonksiyonu başlık dosyasında bildirilmiş:

template< class Lockable1, class Lockable2, class... LockableN >
void lock( Lockable1& lock1, Lockable2& lock2, LockableN&... lockn );

Bu fonksiyonu kullanarak birden fazla mutex'i deadlock oluşturmadan edinebiliyoruz.

std::lock(mtx_1, mtx_2, mtx_3);

- mutex'lerin edinilme sırası tanımlı değil. Yani argüman olarak gönderilen mutex'ler herhangi bir sırada edinilebilirler.
- içsel olarak argüman olarak gönderilen mutex'lerin lock, try_lock ve unlock fonksiyonları çağrılıyor.
- std::lock fonksiyonuna yapılan çağrı thread'i bloke ediyor.
- deadlock oluşmaması garantisi var.

------------------------------------------------------------------------------------------------------

ÖR:
Aşağıdaki kodda deadlock oluşuyor.
Her iki thread'de bloke oluyor.
foo'yu yürüten threadin devam edebilmesi için bar'ı yürüten thread'in b_mtx'i serbest bırakması gerekiyor.
bar'ı yürüten threadin devam edebilmesi için foo'yu yürüten thread'in a_mtx'i serbest bırakması gerekiyor.
Eğer mutex'ler her iki thread tarafından da aynı sırada edinilseydi bir sorun oluşmayacaktı.Birbirini bekliyorlar.

#include <mutex>
#include <iostream>

std::mutex a_mtx;
std::mutex b_mtx;

void foo()
{
	using namespace std::literals;

	a_mtx.lock();
	std::this_thread::sleep_for(100ms); // özellikle beklettikki dead lock oluşsun diye.
	b_mtx.lock();
	std::cout << "func()" << std::endl;
	a_mtx.unlock();
	b_mtx.unlock();
}


void bar()
{
	using namespace std::literals;

	b_mtx.lock();					// önce b yi edindi. Bunu yapmaması gerekiyordu
	std::this_thread::sleep_for(100ms);
	a_mtx.lock();					// şimdi de a yı edindi. 
	std::cout << "bar()" << std::endl;
	b_mtx.unlock();
	a_mtx.unlock();
}


int main()
{
	std::thread t1{ foo };
	std::thread t2{ bar };
	t1.join();
	t2.join();
}

------------------------------------------------------------------------------------------------------

Farklı threadler mutexleri aynı sırada edinselerdi bir sorun olmayacaktı.

#include <mutex>
#include <iostream>

std::mutex a_mtx;
std::mutex b_mtx;

void foo()
{
	using namespace std::literals;

	a_mtx.lock();
	std::this_thread::sleep_for(100ms); 
	b_mtx.lock();
	std::cout << "func()" << std::endl;
	a_mtx.unlock();
	b_mtx.unlock();
}


void bar()
{
	using namespace std::literals;

	a_mtx.lock();					
	std::this_thread::sleep_for(100ms);
	b_mtx.lock();					
	std::cout << "bar()" << std::endl;
	b_mtx.unlock();
	a_mtx.unlock();
}


int main()
{
	std::thread t1{ foo };
	std::thread t2{ bar };
	t1.join();
	t2.join();
}

------------------------------------------------------------------------------------------------------

Diğer seçenekte ise bunun çözümü lock ile değil.Daha iyisi var.

SCOPED LOCK (C++17)
-------------------
Buda bir raii wrapper. Farkı var, bir variadic template ve birden fazla mutexi ctora argüman olarak
olarak geçiyoruz. Kendi içinde deadlock avoidance algoritmasını kullanıyor.

scoped_lock birden fazla mutex'in deadlock olmadan edinilmesini sağlıyor
shared_lock reader/writer lock, yani okuma amaçlı erişim tarafından birden fazla thread okuma amaçlı erişim sağlayabiliyor
Yani bir thread paylaşılan kaynağa yazma amaçlı erişirken birden fazla thread okuma maçlı erişebiliyor


Deadlocktan kaçınmanmın birden fazla yolu var.
1 - Global lock funcını çağırmak. Buda variadic func şablonu. Birden fazla mutex veriyoruz.
	Bir deadlock avoidance garantisi veriyor.

2 - Scoped Lock

#include<mutex>
#include <iostream>

std::mutex a_mtx;
std::mutex b_mtx;

void foo()
{
	using namespace std::literals;

	std::lock(a_mtx, b_mtx);	// Ya ikisinide kilitleyecek yada bloke olacak
	std::lock_guard guard1{ a_mtx, std::adopt_lock };  // lock guard kullanıldı.lock guard kullanmasaydık her ikisinide kendmiz unlock edecekik.
	std::lock_guard guard2{ b_mtx, std::adopt_lock };  // lock guard kullanıldı
	std::this_thread::sleep_for(100ms);
	std::cout << "foo()" << std::endl;
}


void bar()
{
	using namespace std::literals;

	std::lock(a_mtx, b_mtx);
	std::lock_guard guard1{ a_mtx, std::adopt_lock };
	std::lock_guard guard2{ b_mtx, std::adopt_lock };
	std::this_thread::sleep_for(100ms);
	std::cout << "bar()" << std::endl;
}


int main()
{
	std::thread t1{ foo };
	std::thread t2{ bar };
	t1.join();
	t2.join();
}



ASIL YAPI BU DEĞİL SCOPED LOCK KULLANMAK !!!!!!!!!!!!!!!!!!

#include<mutex>
#include <iostream>

std::mutex a_mtx;
std::mutex b_mtx;

void foo()
{
	using namespace std::literals;

	std::scoped_lock guard{ a_mtx, b_mtx }; // CTAD olmasaydı std::scoped_lock<std::mutex,std::mutex> mylock{a_mtx,b_mtx} yazardık.
	std::this_thread::sleep_for(100ms);
	std::cout << "func()" << std::endl;
}


void bar()
{
	using namespace std::literals;
	std::scoped_lock guard{ a_mtx, b_mtx };
	std::this_thread::sleep_for(100ms);
	std::cout << "bar()" << std::endl;
}


int main()
{
	std::thread t1{ foo };
	std::thread t2{ bar };
	t1.join();
	t2.join();
}

------------------------------------------------------------------------------------------------------

ÖR:

#include <mutex>

bool ready_flag;
std::mutex mtx;

void foo() //foo funcı mutexi edindi ve flagi true değerine aldı
{
	std::lock_guard ulock(mtx); 
	ready_flag = true;
}

void bar()
{

	{
		using namespace std::literals;

		std::unique_lock ulock(mtx); // Diğer thread productionu tamamlayınca ready flag true olacak
									 // burada kilit edildiğine göre shared variable olan ready flag kullanılabilir.

		while (!ready_flag)  {
			ulock.unlock();  // burada lock guard olsaydı. Dtor dışında onun sarmaladığı kilidi unlock etmek mümkün değil.
			std::this_thread::yield();  // Önceliği diğer threadlere veriyorum anlamında.s
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // diğer threade şans vermek için bir süre bekliyor.
			ulock.lock(); // tekrar kilidi edindik.
		}
	}

	//...
}

Buradaki problem 100 olan değeri nasıl ayarlayacağımız. Kodda problem yok ama kesin bunu kullanın demiyor hoca.

------------------------------------------------------------------------------------------------------

LOCK FUNC ÖRNEĞİ

#include <mutex>
#include <syncstream>
#include <iostream>

std::mutex m1, m2;

void foo()
{
	std::lock(m1, m2); // hangi mutexin daha önce kilitleneceği garanti değil.
						// Burada her iki kilitte ya edinilecek yada burada bloke olunacak
						// Bir alt satıra gelince her iki kilitte edinilmiş demektir.

	m1.unlock();
	m2.unlock();
	std::osyncstream{ std::cout } << "foo()\n";
}

void bar()
{
	std::lock(m2, m1);

	m1.unlock();
	m2.unlock();
	std::osyncstream{ std::cout } << "bar()\n";
}

int main()
{
	std::thread t1{ foo };
	std::thread t2{ bar};

	t1.join();
	t2.join();
}

------------------------------------------------------------------------------------------------------

ÖR: MÜLAKAT SORUSU - DEADLOCK VAR MI ?

#include <mutex>
#include <thread>
#include <syncstream>
#include <iostream>
#include <chrono>

using namespace std;


struct HasCriticalCode {
	//...
	std::mutex mx;
};


void func(HasCriticalCode& x, HasCriticalCode& y)
{
	x.mx.lock();
	this_thread::sleep_for(1ms);
	osyncstream{ cout } << "get the lock for the object at the address of " << &x << "\n";
	y.mx.lock();
	osyncstream{ cout } << "get the lock for the object at the address of " << &y << "\n";
	y.mx.unlock();
	x.mx.unlock();
}


Aşağıda deadlock neden var ??

int main()
{
	HasCriticalCode a, b;

	thread t1{ func, ref(a), ref(b) };
	thread t2{ func, ref(b), ref(a) };

	t1.join();
	t2.join();
}


Aşağıda Deadlock neden yok ??

int main()
{
	HasCriticalCode a, b;

	thread t1{ func, ref(a), ref(b) };
	thread t2{ func, ref(a), ref(b) };

	t1.join();
	t2.join();
}


*/
