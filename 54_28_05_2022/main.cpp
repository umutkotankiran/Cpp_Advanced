/*

BIR ÖNCEKI DERS KONUŞTUĞUMUZ KONULARI KISACA ÖZETLIYORUZ

Multithread programlarda variables, shared sources, shared data...
Bunları en az bir thread yazma amaçlı kullanacaksa önlem almak gerekiyor.
Eğer bir thread en az yazma amaçlı erişiyorsa ve birden fazla thread varsa
data race olmaması için bazı önlemler almak gerekiyor, arada bir happens before
ilişkisi yok ise.

Ne şekilde koruma yapabiliyoruz.
1 - Kilit sistemleriyle koruma yapabiliyoruz. lock_guard ve mutex kullanımı gibi...
2 - Atomic türleri kullanmak. Bunlar bölünemez işlemler. Yani değişkenşi ya değişmeden
    önceki haliyle görecek, yada değiştikten sonraki haliyle görecek.
    Bazı durumlarda mutex kullanmaya göre atomic değişkenler kullanmak faydalar sağlıyor olabilir.
    Sağladığı faydalar arasında bir mutex kullanmanın dezavantajlarının olduğu 
    yada maliyet açısından kilit sistemlerini doğrudan kullanmanın getirdiği dezavantaj
    Ör: multithread olarak kullanılacak veri yapıları buna iyi örnek olabilir.
	
    Mutex kullanınca critical region denen kod alanını koruma altına alıyoruz.
    Bu alan ne kadar dar tutulursa verim o ölçüde daha yüksek olacak muhtemelen.
    İşte bu alanı daraltmanın belkide en fazla daratabileceğimiz nokta bu alanı
    sadece bir atomic instructiona indirgemek. Bunuda atomic variables larla yapıyoruz.
	
    Atomic değişkenler bize 2 olanak sağlıyor.
    2.1 - Indivisable olduğundan data race ten arındırılmış durumdalar.
    2.2 - Diğeride senkronizsyon aracı olarak kulanılıyor.
	  Bu noktada iş senkronizasyon kısmına gelince happens before ilişkisini
	  hatırlamamız gerekiyor. Bir çok durumda threadler arasında veri alışverişi yani
	  producer/consumer ilişkisi olmalı. Birşekilde bu ilişkiyi düzenlememiz gerekecek.
	  Yani producer veriyi ürettikten sonra consumer o veriyi alıp kullanabilmeli.
	  Bu amaçla atomic değişkenleri kullanıyoruz. Bunun için bir happens before
	  ilişkisi kurmak gerekiyor. Bu happenin before-in time anlamında değil.


HAPPENS BEFORE NEDIR
--------------------
Öyle bir ilişki ki a ile b operasyonları arasında, eğer a operasyona yapıldıktan sonra
b operasyonu onun oluşturduğu yan etkileri görür durumdaysa, yani b operasyonu ona güvenerek yapıldıysa
a operasyonunun sonuçları kesinleşmiş b de bunlardan faydalanacak ise bu garantiye happens before deniyordu.

Aynı thread içerisinde sequence before ilişkisi doğrudan happens before ilişkisini oluşturuyor.
Tek bir thread varsa, tek bir threadde kodların probram sırası zate nsequence before
ilişkisi ile belirleniyor bu da tek bir threadde happens before demek.

Dolayısı ile tek bir thread varsa biz sequence before a güvenerek happens before ilişkisini
kurgulamış oluyoruz. Buna intra thread happens before ilişkisi deniyor.

Bizi daha çok ilgilendiren farklı threadler içinde happens before ilişkisi olması.
A threadinde bir operasyon gerçekleşecek, B threadi bu operasyonun sonuçlarından faydalanacak.
A happens before B. Bu intra thread happens before ilişksi.
Farklı threadler arasında happens before ilişkisinin oluşması için bir senkronizasyon
mekanizması gerekiyor. Bunuda standartlar synchronizes-with ile açıklıyor.

Yani eğer bir threaddeki X noktası ile diğer threaddeki Y noktası arasında bir senkrinoze olma ilişksi
varsa bu aynı zamanda happens before ilişkisini oluşturuyor.

Geçen derste buralarda kaldık.

Not : Syncronizes with ilişkisi runtime a ilişkin bir kavram

Threadler arasındaki senkronizasyon sağlamanın farklı yolları var

Mutex lock/unlock
-----------------
Mutexin lockı ile unlockı arasında bir senkrinozasyon ilişkisi

Thread create/join
------------------
Thread nesnesi için ctorun çalışması tamamlandıktan sonra threadin çalıştıracağı funcın
çağrılmasıyla bir syncronizes with ilişkisi oluyordu. Threadin ctoru öncesinde yapılan
işlevler thread funcı çağrıldığında visible durumdalar
join funcıda aynı senkronizasyon ilişkisini sağlıyor.Bir thread nesnesinin çalıştırdığı
funcı evaluate etmesi noktasıyla join arasında yine bir syncronize ilişkisi var.

Atomic
------
Atomik türlerin aslında birincil faydası operasyonun bölünmez olması.
2 atomik operasyonun birleşimi atomic olmak zorunda değil. 

ÖR: a += 1; in atomic olması farklı(fetch_add ilede yapılıyor)
	
	Ama
	a = a + 1; bu artık atomik bir işlem değil.çünkü burada bir a nın değerini load operasyonu var.
	atomik olan load operasyonu. Buradan elde edilen değer toplama işlemine sokuluyor sonra tekrar
	eşitleme(store) işlemi yapılıyor. load ayrı bir işlem store ayrı bir işlem, bu araya başka işlemler
	girebilir.Atomik olmak zorunda değil bu.
	
	BİR ÖNCEKİ DERSE BU ATOMİK OLMAMA OLAYINIDA EKLE

-------------------------------------------------------------------------------------------------------------------------------------


COMPARE EXCHANGE SWAP İŞLEMİ TEKRARI YAPILDI

Fedor Pikusun atomik konusunda kaynak gösterilen cppconda bir konuşması var. Ona bak.


-------------------------------------------------------------------------------------------------------------------------------------

Bir threadin unlockı ile diğer threadin locku arasında syncronizes with ilişkisi var.
Bunu Release acquire semantiği sağlıyor. Thread diyorki ben değişiklikleri yaptım
bunu haber veriyorum ve bu değişiklikleri yaptığıma dair bir release işlemi yapıyorum.
Diğer taraftan(diğer thread) diyorki bende başka threadin yaptığı değişiklikten
faydalanmak için acquire işlemini yapıyorum. 

bu bir kilit mekanizması ile yapılabildiği gibi atomic işlemlerlede yapılabilir.
ne zaman kilit ne zaman atomik kullanılacak. Bir çok şeye bağlı olabilir.
Performans ölçümü gibi  şeyler yapılabilir.

-------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------

ACQUIRE RELEASE
---------------
C++ 11 ve sonrasında bunu yapabilmek için atomic türler kullanılıyor.
acquirerelease.jpg file githubda
 
Thread 1			Thread 2
--------			--------
X();	       |------------>	B();
A(); ----------|		Y();


If A() syncronizes with B(),
then X() happens before Y()

2 thread arasında bir senkronizasyon ilişksi var ise yani A() operasyonu B() ile
senkronize ise bu durumda A dan önceki işlemle B den sonraki işlem arasında bir happens before
ilişkisi var.


Atomik türleri kullanarak happens before ilişkisini nasıl kullanacağız.
Burada memory order kavramı devreye giriyor.

-------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------

MEMORY ORDER
------------
Atomik türlerde operasyonların neredyese hepsi memory order denen parametre alıyor 
olmasına rağmen biz bunları dikkate almadık. İşin en karmaşık tarafı burası.

ABA(A B A) Problemi
-------------------
Bu çok önemli bir problem. Duruma göre tamamen kodu yanlış kılabilir.
Özellikle lock free veri yapılarının implementasyonunda dikkate alınması gerekiyor.

Normalde CAS işlemi yapılmadan önce, atomik değişlkeni load ediyoruz.
Ondan sonra CAS a giriyoruz.

auto temp = at.load();

//..

at.compare_exchange_strong(temp,desired); // Burası atomik ama bir üstte başka threadler işlem yapabilir
				          // loadun çağrılmasından sonra başka bir thread müdahale edebilir.

Araya giren thread atomik değişkenin değerini değiştirdi fakat compare exchange strong çalışmaya
başlamadan önce tekrar değiştirdi eski haline getirdi.
Yani aşağıdaki gibi

A 
auto temp = at.load();

B // araya giren thread değişkeni değiştirdi
A // Burada tekrar eski haline getirdi.

at.compare_exchange_strong(temp,desired);

ABA problemi yukarıdaki. Bu durumda CAS funcı çağrıldığında değer değişmemiş gibi algılayacak.
Burada değişkenin önce B ve en son A yapılmasının başka problemlere yol açmasıda mümkün ayrıca.

-------------------------------------------------------------------------------------------------------------------------------------

MEMORY ORDERIN ENUMERATOR DEĞERLERI

std::memory_order_seq_cst
std::memory_order_acq_rel
std::memory_order_acquire
std::memory_order_release
std::memory_order_consume
std::memory_order_relaxed



C++ 20 İLE INLINE VARIABLE LAR TANIMLANMIŞ

inline constexpr memory_order memory_order_relaxed = memory_order::relaxed;
inline constexpr memory_order memory_order_consume = memory_order::consume;
inline constexpr memory_order memory_order_acquire = memory_order::acquire;
inline constexpr memory_order memory_order_release = memory_order::release;
inline constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
inline constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;

ÖR:
#include<iostream>
#include<atomic>

int main()
{
	std::atomic<int> x;

	x.store(1, std::memory_order::seq_cst);

	x.store(1,std::memory_order_seq_cst); // C++20
}

-------------------------------------------------------------------------------------------------------------------------------------

6 Tane memory order olsada semantik yapı olarak 3 tane model var.

CPPREFERENCE NOTLAR
-------------------
Value					Explanation
------------------------------------
memory_order_relaxed : 	Relaxed operation: there are no synchronization or ordering constraints imposed 
			on other reads or writes, only this operation's atomicity is guaranteed 
			(see Relaxed ordering below)

memory_order_consume :  A load operation with this memory order performs a consume operation 
			on the affected memory location: no reads or writes in the current thread 
			dependent on the value currently loaded can be reordered before this load.
			Writes to data-dependent variables in other threads that release the same 
			atomic variable are visible in the current thread. On most platforms, 
			this affects compiler optimizations only (see Release-Consume ordering below)

memory_order_acquire : 	A load operation with this memory order performs the acquire operation on 
			the affected memory location: no reads or writes in the current thread can 
			be reordered before this load. All writes in other threads that release the
			same atomic variable are visible in the current thread (see Release-Acquire ordering below)

memory_order_release :	A store operation with this memory order performs the release operation:
			no reads or writes in the current thread can be reordered after this store. 
			All writes in the current thread are visible in other threads that acquire 
			the same atomic variable (see Release-Acquire ordering below) and writes 
			that carry a dependency into the atomic variable become visible in other 
			threads that consume the same atomic (see Release-Consume ordering below).

memory_order_acq_rel : 	A read-modify-write operation with this memory order is both an acquire 
			operation and a release operation. No memory reads or writes in the current 
			thread can be reordered before the load, nor after the store. 
			All writes in other threads that release the same atomic variable are 
			visible before the modification and the modification is visible in other 
			threads that acquire the same atomic variable.



memory_order_seq_cst :	A load operation with this memory order performs an acquire operation, 
			a store performs a release operation, and read-modify-write performs 
			both an acquire operation and a release operation, plus a single 
			total order exists in which all threads observe all modifications in 
			the same order (see Sequentially-consistent ordering below)

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

DERSTEKI NOTLARDAN DEVAM

memory_order_seq_cst : Tüm operasyonlar için load(read) yada store(writee) operasyonuysa default değer bu.

memory_order_relaxed : Read, modify, write işlemlerinde ise 2 tane birden memory order değerine ihtiyaç var.
		       Hem okuma hemde yazma tarafı için.


- Bir read işlemi yapılacaksa aşağıdakiler geçilebilir
  sequentially consistent, acquire, consume, relaxed.

- Bir write işlemi için,
  sequentially consistent, release, relaxed.

- Read-modify-write için,
  Yukarıdakilerden herhangibirini yada acquire_release i kullanabiliriz.


Memory Orderda 4 temel model var
Bu ordering olayı geçen derste de bahsedildi.
İşlemleri sıraya sokabiliyor ama her işlem kendi threadinde belli sıralarda oluyordu.
Bu konudan önce oradaki notları okumak gerek.

Sequencial Consistency 
----------------------
Bu model kullanmaktaki amaç syncronization + order sağlamak

Acquire Release
---------------
Bu modeldeki amaç ise syncronization.

Bu yazı aşağıdaki ömrnektede yazıldı !!!!!
Eğer bir atomic işlem bir atomic değişken üzerinde release modda store yaparsa
aynı atomic değişken sonra başka bir thread tarafından okunduğunda o okumada acquire modelinde yapılırsa
oradaki store işlemiyle diğer load işlemi arasında bir senkronizasyon oluşacak.

Comsume Release
---------------
birşey yazmadık

Relaxed
-------
Ordering ve Senkronizasyon yok.Senkronizasyon yok derken threadler arası senkronizasyon yok
Sonuçta atomic kullanacaksak orada zaten bölünmezlik var.
Birinin işlemi biter diğeri başlar gibi durum var.

-----------------------------------------------------------------------------------------------

ÖR: Sequencial Consistency örneği

Sequencial consistency hem senkronizasyon hemde ordering sağlıyor demiştik.

foo baz bar func sırasiyla calismis olsunlar
Assertion'lar fail olabilir mi?

#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int> x{ 0 }, y{ 0 };

void foo()
{
	x.store(1, std::memory_order_seq_cst); // memory_order_seq_cst zaten default parameter
						// seq con. kaç thread olursa olsun, tüm threadler sanki
						// program akışını tek bir thread olarak görüyor.Araya girebilir 
						// farklı threadlerdeli işlemler ama tüm threadler aynı şekilde görüyorlar
						// programın tek bir threadde çalışması gibi düşünülebilir.

// Önce foo çalışmışsa x e 1 değerini store edecek ve artık diğer threadlerde 
// görünür durumda olacak
											
}

void baz()
{
	assert(x.load() == 1 && y.load() == 0);
	//2. olarak bu çağrılır. x 1 olmuştu. doğru.
	// y de 0 durumda.
}

void bar()
{
	y.store(1, std::memory_order_seq_cst);
	// y ye 1 değerini verdi.
}

void func()
{
	assert(x.load() == 1 && y.load() == 1); // Burasıda ok
}

int main()
{
	std::jthread{ foo };
	std::jthread{ baz};
	std::jthread{ bar};
	std::jthread{ func};
}


-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

Acquire release ise bir senkronizasyon sağlıyor. Ordering yok.

Relaxed ta bunların hiçbiri yok.

atomic x = 0; // bu bu işlemi relaxed modda yaparsam(memory order parametresi olarak relaxed geçersem)
			  // ben bunu senkronizasyon amaçlı kullanmayacağım ve orderinglede işim yok demek.

void func()
{
	x.store(1, memory_order_relaxed); // Bu işlem atomic ve bu konuda problem yok. Data race yok
									  // indivisable bir operation. 
									  // Senkronizasyon ise yok !!!
									  // Ordering konusunda da garanti yok !!!!

}

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

ÖR:

memory order relax burada counter gibi bir örneği artırırken kullanılabilir.
Orderingede sync e de gerek yok ise kullanılabilir. Senkronizasyon yok derken
threadler arası senkronizasyon yok. Yoksa işlem atomic yapılıyor burada.
Atomic te bölünmezlik var. Biri artırır sonra diğeri artırır.
Senkronizasyon Acquire Release yapısında devreye girecek.

#include <atomic>
#include <cassert>
#include <thread>

using namespace std;

atomic<int> count{0};

void func()
{
	for(int i = 0; i < 1000; ++i)
	{
		count.fetch_add(1,memory_order_relaxed);
	}

}

void foo()
{
	jthread t1{func};
	jthread t2{func};
	jthread t3{func};
	jthread t4{func};
	jthread t5{func};

}

int main()
{
	foo();

	std::cout<< count.load() << '\n'; // loada memory order relax argümanını verebilirdik.
}

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

Acquire Release Örneği

Eğer bir atomic işlem bir atomic değişken üzerinde release modda store yaparsa
aynı atomic değişken sonra başka bir thread tarafından okunduğunda o okumada acquire modelinde yapılırsa
oradaki store işlemiyle diğer load işlemi arasında bir senkronizasyon oluşacak.




using namespace std;

int ival = 17;
atomic guard = 0;

void func()
{
	//Operation A
	ival = 98;
	guard.store(1,memory_order_release); // Dikkat Memory order release !!!!!! 
	
	// Ben veriyi hazırladım ve bunu release ediyorum.
	// Yani atomic değişkenden okuma yapacak diğer threadlere
	// bunu bildiriyorum anlamında.
	// 1. Bu senkronizasyon runtime a ilişkin. kaynak koda ilişkin değil
	// Bu işlemin yapılmasından sonra başka bir thread bizim atomik değişkenimizden
	// load işlemi yaparsa yani okuma yaparsa ve bu okuma acquire memory order ile yapılırsa
	// bir senkronizasyon oluşacak. Yani acquire ile load eden operasyon ile store arasında
	// bir senkronizasyon oluşması demek, acquire den sonraki işlemden
	// bunun(ival = 98 in) daha önce gerçekleşmesi demek.

	// Yani pratikte şu, herhangibir thread eğer bizim ival = 98 de oluşan sonucun
	// visible olmasına dayanarak bir işlem yapacaksan tek yapman gereken atomic değişkenin
	// değerini memory order acquire ile load etmen yeterli. Bu durumda load işleminden sonra 
	// buradaki bütün store işleminden önceki bu thrreaddeki program sırasında yapılan işlemler
	// artık orada visible olacak.

	// Yani guard.store ile değeri 1 verildikten sonra, ival = 98 işleminin yapıldığından emin olacak.
	// Syncronizes with ilişkisi happens before sonucunu doğuracak.
}

void foo()
{
	// Operation B
	while(!guard.load(memory_order_acquire))  // Dikkat memory order acquire
		;
	
	// Döngüden çıktıktan sonra buradaki load ile diğer func içerisindeki store senkronize olduğundan
	// biz 1 değerini okuduğumuzda artık ival = 98 işleminin gerçekleşmiş olduğunun garantisini elde ediyoruz.
	// A Happens before B garantisi elde edildi.

	std::cout << "ival : " << ival << '\n';
}


int main()
{
	std::thread t1{func};
	std::thread t2{foo};
	t1.join();
	t2.join();
}

-----------------------------------------------------------------------------------------------

Acquire Release Genel Yapı

Aşağıdaki kodlar multithread olarak çalışıyor.
Burada AcquireRelease2 isimli görsele bak.
Happens before ilişkisivar burada.

preshing.com

#include <ctime>
#include <atomic>

// payload
 struct Message {
	std::clock_t tick;
	const char* str;
	void* param;
};

Message g_payload;
std::atomic g_guard{ 0 };

void SendTestMessage(void* param)
{
	// Copy to shared memory using non-atomic stores.
	g_payload.tick = clock();  // önce bu non atomic 3 işlem yapıldı
	g_payload.str = "TestMessage";
	g_payload.param = param;

	// Perform an atomic write-release to indicate that the message is ready.
	g_guard.store(1, std::memory_order_release); // store işlemini memory order release ile yaptıki senkronizasyon olsun
}


bool TryReceiveMessage(Message& result)
{
	// Perform an atomic read-acquire to check whether the message is ready.
	int ready = g_guard.load(std::memory_order_acquire); // Burada da acquire kullanılmış. Bu func sürekli çağrılacak yukarıdaki gibi while döngüsü var gibi düşün.
														 // bu thread  yukarıdaki thread/func işini yapmasını beklemiş oluyor aslında
														 // yukarıdaki işini yapınca bu devreye girecek.

	if (ready != 0)
	{
		// Yes. Copy from shared memory using non-atomic loads.
		result.tick = g_payload.tick; // Non atomic olsada bunlar sonuçları visible hale geldi. 
		result.str = g_payload.str;
		result.param = g_payload.param;

		return true;
	}

	// No.
	return false;
}

Burada 2 ayrı thread arasında happens before ilişkisi var.

-----------------------------------------------------------------------------------------------

Acquire release3 isimli resimde
Yukarı veya aşağı yönde kaydırılabilir ve kaydırılamaz işlemler var.

MOLA

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

AcquireRelease4 isimli dosyaya bak.
Burada hiçbir türlü fail yok.

İşlemcilerin çoğunda derleyicinin bazı optimizasyonunu engelliyor.
Bir bariyer kullanıyor. Bir noktadan bir noktaya kodları taşıyamıyor ama ters yönde reordering yapıyor.
Bir önceki resimde idi sanıyorum.

-----------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <iostream>

class SpinLockMutex {
public:
	SpinLockMutex()
	{
		m_f.clear();
	}
	void lock()
	{
		while (m_f.test_and_set(std::memory_order::acquire)) // yine acquire release olayı kullanılıyor.
			; //null statement
	}

	void unlock()
	{
		m_f.clear(std::memory_order::release);  // Dİkkat!!!
	}
 private:
	std::atomic_flag m_f;
};

SpinLockMutex mtx;
unsigned long long counter{};

void func()
{
	for (int i{ 0 }; i < 100'000; ++i) {
		mtx.lock();
		++counter;
		mtx.unlock();
	}
}

int main()
{
	std::vector<std::thread> tvec;

	for (int i = 0; i < 10; ++i) {
		tvec.emplace_back(func);
	}

	for (auto &th : tvec) {
		th.join();
	}

	std::cout << "counter = " << counter << "\n";
}

1:40 condition variable ile acquire release karşılaştırması var.

-----------------------------------------------------------------------------------------------

ÖR: CAS örneği

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

std::atomic is_ready{ false }; //CTAD
std::atomic is_done{ false };
std::atomic go_count{0};

 void lottery(std::string name)
 {
	++go_count;

	while (!is_ready)
		;

	for (volatile int i = 0; i < 20000; ++i)
	{

	}
	bool expected{ false };
	if (is_done.compare_exchange_strong(expected, true)) // 1:48-1:49 Sadece bir tanesi girecek incele
	{
		std::cout << "kazanan: " << name << '\n';
	}
}

int main()
{
	constexpr const char* pnames[] = { "necati", "akif", "ahmet", "harun", "mehmet", "dogukan"};
	std::vector<std::jthread> tvec;
	for (auto p : pnames) {
		tvec.emplace_back(lottery, p);
	}

	while (go_count != std::size(pnames))
		;
	is_ready = true;
	for (auto& t : tvec) {
		t.join();
	}
}


-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

RELEASE CONSUME

Release Consume biraz farklı ve eleştiriliyor. Productionda en çok acquire release kullanacağız
Bazende relaxed kullanacağız.
Require release de bazı operasyonlar kaydırılabiliyordu bazıları ise kaydırılamıyor.
AcquireRelease3 isimli resimde var.

Release consume da sadece atomik değişkene bağlı dependent işlem için bir garanti var.
Diğer işlemler için bu garanti yok.

Cppreferenceta bu örneği vermişler

#include <thread>
#include <atomic>
#include <cassert>
#include <string>

std::atomic<std::string*> ptr;
int data;

void producer()
{
	std::string* p  = new std::string("Hello");
	data = 42;
	ptr.store(p, std::memory_order_release);
}

void consumer()
{
	std::string* p2;
	while (!(p2 = ptr.load(std::memory_order_consume)))
		;

	assert(*p2 == "Hello"); // never fires: *p2 carries dependency from ptr

	assert(data == 42); // may or may not fire: data does not carry dependency from ptr
						// datanın 42 olma garantisi yok.Olay bu.Sadece atomik değişkenler için yapılıyor.
}

int main()
{
	std::thread t1(producer);
	std::thread t2(consumer);
	t1.join(); t2.join();
}

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

C++ 17 İLE BAZI STL ALGORİTMALARI PARALEL OLARAK ÇALIŞABİLİYOR
Örneğin sort algoritmasını paralel çalıştırıyoruz ama paralel çalışma garantisi yok
böyle bir talepte bulunuyoruz ve buna derleyici karar veriyor.
Normalde bizim stl algoritmalarımız 1 thread çalışıyor ama özel bir parametreyele bu talebi yapıyoruz.
Hepsini değil ama 70 civarı algoritmayı paralel çalıştırma olanağı sağlandı. Burada nasıl kod olacağı derleyiciye bağlı

execution header file

#include <string>
#include <iostream>
#include <vector>
#include <execution>
#include <algorithm>
#include <random>

constexpr int n = 10'000'000;
using dsec = std::chrono::duration<double, std::milli>;

int main()
{
	std::vector<int> ivec(n);

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution dist{ 0,999'999 };
	generate_n(ivec.begin(), n, [&] {return dist(mt); });

	auto tp_start = std::chrono::steady_clock::now();

	sort(ivec.begin(), ivec.end());

	auto tp_end = std::chrono::steady_clock::now();

	std::cout << dsec{ tp_end - tp_start }.count() << " milliseconds\n";

}

------------------------------------------------------------------------------------------------

ŞİMDİ AYNISINI PARALEL OLARAK ÇALIŞTIRALIM

Derleyici bunları paralel olarak çalıştırmak zorunda değil.

3 tane global değişken var. Bunlar enumerator değil.
İlk parametre execution policy olacak.
std::execution::seq;
std::execution::par;
std::execution::par_unseq

Aşağıdakiler aynı anlamda
sort(ivec.begin(),ivec.end()); 
sort(std::execution::seq,ivec.begin(),ivec.end());

Bu durumda ise paralel çalıştırmış oluyoruz.
sort(std::execution::par,ivec.begin(),ivec.end());

Bu ise derleyiciler tarafından çok implemente edilmemiş.
Her algoritma için uygun olmayabiliyor.Standartlarda yazıyor hangisi için olabileceği.
vektörizasyon komutlarını kullanabilirsin ve işlem sırasınında önemi yok demek.
Bunun çok fazla bir etkisi genelde olmuyor.
sort(std::execution::par_unseq,ivec.begin(),ivec.end()); 

Algoritların çalışması sırasında exception throw ederse terminate çağrılır !!!!!!!

ÖR:

#include <string>
#include <iostream>
#include <vector>
#include <execution>
#include <algorithm>
#include <random>

constexpr int n = 10'000'000;
using dsec = std::chrono::duration<double, std::milli>;

int main()
{
	std::vector<int> ivec(n);

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution dist{ 0,999'999 };
	generate_n(ivec.begin(), n, [&] {return dist(mt); });

	auto tp_start = std::chrono::steady_clock::now();

	//sort(ivec.begin(), ivec.end());
	sort(std::execution::par,ivec.begin(),ivec.end());

	auto tp_end = std::chrono::steady_clock::now();

	std::cout << dsec{ tp_end - tp_start }.count() << " milliseconds\n";

}

Hocanın pc de 4 kat farketti.

------------------------------------------------------------------------------------------------

replace_if örneği

#include <string>
#include <iostream>
#include <vector>
#include <execution>
#include <algorithm>
#include <random>

constexpr int n = 10'000'000;
using dsec = std::chrono::duration<double, std::milli>;

int main()
{
	std::vector<int> ivec(n);

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution dist{ 0,999'999 };
	generate_n(ivec.begin(), n, [&] {return dist(mt); });

	auto tp_start = std::chrono::steady_clock::now();

	//replace_if(ivec.begin(),ivec.end(),[](int x){ return x % 2 == 0 }, 0);
	replace_if(std::execution::par,ivec.begin(),ivec.end(),[](int x){ return x % 2 == 0 }, 0);

	auto tp_end = std::chrono::steady_clock::now();
	std::cout << dsec{ tp_end - tp_start }.count() << " milliseconds\n";

}

replace_if te 6-7 kat farketti

------------------------------------------------------------------------------------------------

Burada da replace if par_unsec için yazıldı

#include <string>
#include <iostream>
#include <vector>
#include <execution>
#include <algorithm>
#include <random>

constexpr int n = 10'000'000;
using dsec = std::chrono::duration<double, std::milli>;

int main()
{
	std::vector<int> ivec(n);

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution dist{ 0,999'999 };
	generate_n(ivec.begin(), n, [&] {return dist(mt); });

	auto tp_start = std::chrono::steady_clock::now();

	//replace_if(ivec.begin(),ivec.end(),[](int x){ return x % 2 == 0 }, 0);
	replace_if(std::execution::par_unsec,ivec.begin(),ivec.end(),[](int x){ return x % 2 == 0 }, 0);

	auto tp_end = std::chrono::steady_clock::now();
	std::cout << dsec{ tp_end - tp_start }.count() << " milliseconds\n";

}

7-8 kat daha iyi.

------------------------------------------------------------------------------------------------










































































*/
