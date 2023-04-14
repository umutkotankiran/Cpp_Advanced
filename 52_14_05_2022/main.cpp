/*

Bugün Once flag ve Call once tan bahsedilecek.
Ayrıca Conditional variable dan bahsedilecek.
Memory Modelden bahsedilecek
Bir sonraki ders atomikten bahsedilecek.

----------------------------------------------------------------------------------------------------------------------

Multithread programlarda öyle durumlar oluyorki, birden fazla thread sözkonusu olmasına rağmen
bir functionun sadece 1 kere çağrılması gerekiyor.

Bu özellikle paylaşılan bir değişkene ilk değer verme sözkonusu olduğunda önemli.
Yada ilk defa atama yapmada önemli. Bu konuda önlem almamız gerekiyor.

Yani 1 den fazla threadin bir değişkeni 2 kere initialize yada 2 kere assingment yapmasını engellememiz
gerekiyor.

Ör: dinamik ömürlü değişkenin 1 kere hayata gelmesini istiyoruz ama multithread çalışan bir kodda 
1 den fazla thread o koda gelirse 2 kere nesne yaratmaya çalışabilir. Program mantığı gereği 1 nesne
gerekiyor olabilir.

Burada standart library nin once_flag isimli türü var
birde call once isimli func şablonu var.

Once flag, functionun 1 kere çağrılmasını sağlıyan bir flag
Call once, once_flagi kullanarak çağrılacak callable ı ve ona gönderilecek argümanları alıyor.
Dolayısı ile genel yapıda, önce once flag türünden değişken tanımlıyoruz ve bunu call once a argüman
olark gönderiyoruz. Call once ı variadic parametresine forward edilcek argümanları gönderiyoruz yani.

Arka tarafındaki kod yapısı bu funcın 1 kere çağrılmasını sağlıyor.
Call once yapısına ilk gelen thread funcın çağrılmasını sağlıyor.Diğerleri o arada bloke oluyor.
Ne zaman func çağrısı tamamlanırsa diğer threadler devam ediyorlar.

----------------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::once_flag gflag;

void func(int id)
{
	using namespace std::literals;

	std::this_thread::sleep_for(500ms);
	std::call_once(gflag, [id]() {
		std::cout << id << " threadi icin cagrildi\n"; });

	///...
}

int main()
{
	std::vector<std::thread> tvec;

	for(int i = 0; i < 10; ++i)
	{
		tvec.emplace_back(func,i);
	}

	for(auto &th : tvec)
		th.join();
}

1 den fazla thead var olmasına rağmen call_once içerisindeki closure type bir kere çağrıldı.

Sadece 0 için çıktı mesela ama deterministik değil.
Diğer denemede 2 çıktı
diğerinde de 6

----------------------------------------------------------------------------------------------------------------------

Call once ı aynı once flag için birden fazla kez çağırabiliriz.
Birden fazla çağırıp birden fazla callable verirsek bunların ikisi üçü birden çağrılacak gibi
bir anlam çıkmıyor.

Yani bir once_flag farklı funclar için call_once çağrısı içerisine yazılsa dahi 1 kere çağrılıyor.


ÖR:

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::once_flag gflag;
using namespace std::literals;

void foo()
{
	std::this_thread::sleep_for(100ms);
	std::call_once(gflag, []() {					// DİKKAT! Aynı Once flag değişkeni
		std::cout << "registered in foo\n"; });

	///...
}

void bar()
{
	std::this_thread::sleep_for(100ms);
	std::call_once(gflag, []() {					// DİKKAT! Aynı Once flag değişkeni
		std::cout << "registered in bar\n"; });

	///...
}

 
int main()
{
	std::thread a[10];

	for(int i = 0; i < 10; ++i)
	{
		a[i] = i % 2 == 0 ? std::thread{ func } : std::thread{ foo };
	}

	for(auto &th : a)
		th.join();
}

Aynı once flag değişkeni kullanıldığı için hangisinin daha önce call once a gireceğini bilmiyoruz.
Hangisi girerse girsin ya üstteki lambda 1 kere çağrılacak
yada alttaki bar 1 kere çağrılacak.

----------------------------------------------------------------------------------------------------------------------

Singleton yapısına bakalım

#include <mutex>
#include <iostream>
#include <syncstream>
#include <vector>


class Singleton
{
public:
	
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static Singleton* get_instance()
	{
		std::call_once(m_init_flag,init);
		return m_instance;
	}

	static void init()
	{
		m_instance = new Singleton;
	}

private:
	inline static std::once_flag m_init_flag;
	inline static Singleton *m_instance{};
	Singleton() = default;
};

void func()
{
	std::osyncstream{std::cout} << Singleton::get_instance();
}

int main()
{
	std::vector<std::thread> tvec;

	for(int i = 0; i < 100; ++i)
	{
		tvec.emplace_back(func);
	}
	
	for(auto &t : tvec)
		t.join();
}

Çıktıya aynı adresi yazar.


----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------


CONDITION VARIABLE
------------------
Multithread programlamada en sık ihtiyaç duyulan kod yapılarından biri, bir threadin diğer thread tarafından
üretilen veriyi kullanması. İllaha bir veri olmasıda gerekmiyor.Veri tipik bir durum.
Bu thredin bazı işlemleri yapıp diğereinin işini yapcak duruma getirmeside olabilir.
Sonuçta bir threadin çalışması için başka bir threadin başka işlemleri gerçekleştirmiş olması 
gerekiyor. 

Eğer olay sadece birinin değer üretip diğer threade iletmesi olsaydı bunun için feature mekanizmasını
kullanabilirdik.

Burada ilk akla gelen yöntem, bir tane değişken olur(int veya boolean bir değişken olur) producer olan
thread işini gördüğünde, örneğin boolean değeri false tan true ya dönüştürür, diğer threadde bu
boolean değişkeni gözler(hem consumerın hem producerın bu değişkene senkronize şekilde erişmeli)
Ne zaman bayrak set edilirse, çalışmaya devam edilir. Bayrak değişken set edilene kadar bir şekilde işlem yapmam sözkonusu değil.
Ne zamanda producer üretimini gerçekleştirirse, consumer yoluna devam eder.

----------------------------------------------------------------------------------------------------------------------

ÖR: İhtiyaç noktasını gösteren örneği yazıyoruz.
Neden istenmediğinide yazacağız

#include <mutex>

int shared_variable{};
std::mutex mtx;

void producer()
{
	using namespace std::literals;
	
	std::this_thread::sleep_for(1000ms);
	std::lock_guard lk{mtx};
	
	// ... production code
	
	shared_variable = 999;

}


void consumer()
{
	std::unique_lock ulock{mtx};

	while(shared_variable == 0)
	{
		ulock.unlock();  // unlock edildi

		// std::this_thread::yield(); //daha optimize olarak bu yapılabilir.
		//std::this_thread::sleep_for(1000ms); burasıda optimizasyon kodu

		ulock.lock();  // tekrar lock edildi.
	}

	//consumption
	// Burada shared variable değerini almış durumda ve kullanıyoruz.

	std::cout << "the value is : " << shared_variable << '\n';

}

int main()
{
	std::thread t1{producer};
	std::thread t2{consumer};

	t1.join();
	t2.join();
}

Bu kod gereksiz iş kaybına yol açıyor.
Sürekli shared variable ı gözlemek için kullanıyoruz. 
aynı zamanda sürekli kilidi edinip serbest bırakıyoruz.
Task tamamlanana kadar tamamen bloke olsaydı işlemci zamanını boşa harcamayacaktı


----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------

Task tamamlandığında condition variable uyandırma işini yapacak.
Biz bekleme süreci içinde diğer threadin işi yapması sürecinde beklemede kalacağız ne zaman o iş tamamlanınca
bizim thread o zaman uyanıp işini yapacak. Burada kullanılan bir condition olması gerekiyor.

Task tamamlanınca taski yürüten thread o değişken vasıtasıyla işin bittiğini haber verecek.
Buradaki senkronizasyon için condition variable türünden değişkene ihtiyaç var. 

GİTHUB NOTLARI !!!!!!!!!!!!!!!!!!!!!!!!
---------------------------------------
- Condition variable bir başka thread'in tetikleyeceği bir event'i beklemenin temel mekanizmasıdır. (Yani bir Synchronization primitive)

- Birden fazla thread'in kullanıldığı programlarda çoğu zaman bir thread'in belirli bir event oluşuncaya kadar başka bir thread'i beklemesi gerekir 
  (giriş çıkış işleminin tamamlanması ya da bir verinin hazırlanması gibi)

- Bir thread'in bir event'in gerçekleşip gerçekleşmediğini sürekli olarak sorgulaması, işlemci zamanının boşa harcanmasına, 
  dolayısıyla verimin düşmesine neden olabilir.

- Böyle durumlarda bekleyen thread'in bloke olması ve ilgili event gerçekleştikten sonra thread'in tekrar çalışmaya başlaması genellikle daha iyidir.
  Yukarıda yazdığımız koda bir optimizasyon aslında condition variable.

- Bir condition variable, belirli bir koşul (condition) gerçekleşinceye kadar bir veya daha fazla thread'in (bloke olarak) beklemesini sağlayan bir senkronizasyon yapısıdır.

- Bir condition variable bir event ile ilişkilendirilir.

- Bir event'i beklemek isteyen bir thread, condition variable'ın wait fonksiyonlarından birini çağırır. (wait, wait_for, wait_until)

- Bir thread (producer) bir event'in gerçekleşmiş olduğunu bir ya da birden fazla thread'e (consumers) bildirmek için condition variable'ın 
  notify_one ya da notify_all fonksiyonlarından birini çağırır !!!!! Notify_one ile bir threade notify_all ise birden fazla thredin uyandırılmasını sağlar.
  Buradaki olay, bekleme sırasında boşyere uyanık kalıp işlemci zamanını almaması böylece işlemci zamanı
  daha faydalı işler için daha verimli olarak kullanılsın.

- Bildirimi alan thread çalışmaya devam ettiğinde istenen koşulun sağlanmış olma garantisi yoktur. Başka bir thread koşulun değişmesini sağlamış olabilir ya da 
  "spurious wakeup" denilen durum oluşmuş olabilir. (Bekleyen bir thread'in aslında diğer taraftan bir bildirim almadan uyanmasına "spurious wake" denir.)
  
- Bu bildirimin/sinyalin(producerın işini yaptığının sinyali, kim beni bekliyorsa uyansın ve devam etisn sinyali) sağlıklı bir şekilde iletilmesi için
  sinyalin kaçırılma riskinden ziyade implementasyon tarafında şunu feda ediyorlar. Öyle durumlar olacakki biz threadi uyandıracağız ama
  koşular sağlanmış değil.Koşul sağlanmamış olmasına rağmen bir notification olmadan karşı taraf uyandırılırsa buna spurious wakeup
  deniyor.

- Programcı gözüyle bu tamamen random ve öngörülemez.

- Bu nedenle burada thread uyandığı zaman gerçekten task tamamlandı ve ben o yüzden mi uyandım yoksa ortada
  bir spurious wakeup mı var.Bunun için condition kullanılıyor. 
  Condition variable ile buradaki conditionu birbirine karıştırmayalım.Bu bir boolean veri olabilir
  int değişken olabilir.Stack queue gibi library de empty funcı bu görevi yapabilir.

- En tipik olarak condition bir paylaşılan değişken ve bu shared variable diğer üretici thread görevini tamamlayınca bunu set edecek
  tüketici ise uyandığında(notification yapıldığında) o da gidip kilit altında o değişkene bakacak ve set edildiğine göre
  bu spurious wakeup değil diyecek ve görevini yapacak. Spurious wakeup ın olmama ihtimali yok.
  Kontrol hep yapılmalı !!!!!!!!!!!!!


CONDITION VARIABLE BURADA SENKRONIZASYON PRIMITIVE GÖREVINDE
--------------------------------------------------------------
- Threadler arası iletişimin kurulması için std::condition_variable türünden bir nesneye ihtiyacımız var

- condition_variable olarak std::condition_variable sınıfı türünden bir nesne kullanılır.

- condition_variable sınıfı <condition_variable> başlık dosyasında tanımlanmıştır. 
  std::condition_variable nesneleri kopyalanamaz ve taşınamaz (not copyable - not moveable).

- sınıfın wait, wait_for ya da wait_until üye fonksiyonları ile bekleyecek thread bloke edilir koşulun sağlanması beklenir.

- sınıfın notify_one ve notify_all üye fonksiyonları ile bekleyen thread'lere koşulun oluştuğu bildirilir (signal).

- uyanan thread'in koşulu tekrar sınaması gerekir, çünkü
	-- spurious wakeup oluşabilir.
	-- sinyalin alınması ve mutex'in edinilmesi zaman aralığı içinde başka bir thread koşulu değiştirmiş olabilir.

- wait, wait_for ve wait_until fonksiyonları mutex'i atomik olarak edinirler ve ilgili thread'i bloke ederler.

- notify_one ve notify_all fonksiyonları atomiktir.


TIPIK IŞLEM AKIŞI ŞÖYLE GERÇEKLEŞTIRILIR
----------------------------------------
Evet'i gerçekleştirecek ve diğer thread'e bildirimde bulunacak thread,
- Tipik olarak std::lock_guard kullanarak bir mutex'i edinir. Burada producer tarafı için raii olarak unique guard ve scope guardda kullanılabilir.
  Producer için raii ler fark etmiyor ama consumerlar için fark ediyor.

- Koşul değişkenini(paylaşılmış) kilit edinilmiş durumdayken set eder. Kilit mekanizması gerekiyor.
  Hem consumer hemde producerın aynı mutexi kullanması gerekiyor. Kilidi edindim görevi tamamladım ve conditionu set ettim.
  Yapılan değişikliğin bekleyen thread(ler)e doğru bir şekilde bildirilebilmesi için, 
  paylaşılan değişken atomik olsa dahi değişikliğin kilit edinilmiş durumda yapılması gerekir.

- Bu amaçla tanımlanmış olan std::condition_variable nesnesinin notify_one ya da notify_all fonksiyonlarından birini çağırır. 
  Bu fonksiyonlar çağrıldığında kilitin edinilmiş durumda olması gerekmez. 
  Bu funclardan birini çağırınca kilit altında bu çağrının yapılma mecburiyeti yok. Condition set edilirken kiliti edinmiş olması gerekiyor
  consumer tarafın ama notify işlemi yapılırken kilit altında yapılması gerekli değil hatta kilit altında yapılmaması daha iyi.
  
- Eğer bu fonksiyonlar kilit edinilmiş durumda çağrılırsa bildirim alan thread'ler kilidi edinemezler ve tekrar bloke olurlar.


BEKLEYEN TARAF NE YAPIYOR
-------------------------
İki yöntem var. İkiside aynı ama biri diğerinin hazır hale getirilmiş hali.

Alt seviye için, bekleyen bir thread, önce std::unique_lock kullanarak (aynı) mutex'i edinir.Unique lock mecburi.
Kendi raii sınıfımızı kullanmak istersek condition_variable_any var.Bunda basic locable özelliğini sağlayan herhangibir kilit sistemini kullanabiliyor.

Daha sonra aşağıdaki iki seçenekten birini uygular:
- Birinci seçenek
	-- Değişikliği zaten yapılmış ve bildirimin de gerçekleşmiş olabileceği ihtimaline karşı önce koşulu test eder.
	   Kilidi edindik ama edindiğimizde zaten işlemler yapılmış shared variable değeri değiştirilmiş ve iş bitmiştir.
	   Biz burada beklemeye geçersek ömür boyu bekleriz :D:D Böyle duruma ise lost wakeup deniyor.
	   İlk önce consumer taraf ben kilidi aldım ama önce bakayım hali hazırda condition set edilmiş mi?
	   set edilmişse iş bitmiştir, beklemesine gerek yok. Set edilmemişse uykuya dalacak. Uykuya dalmak için
	   condition_variable ın wait funclarından biri çağrılıyor.

	-- wait, wait_for, ya da wait_until fonksiyonlarından birini çağırır. Çağrılan wait fonksiyonu edinilmiş mutex'i otomatik olarak serbest bırakır ve thread'in çalışmasını durdurur.
	   wait çağrıldığında mutex otomatik olarak serbest bırakılıyor.Yani kilit serbest bırakılmış oluyor.
	   
	-- condition_variable nesnesinin notify fonksiyonu çağrıldığında (ya da bekleme süresi dolduğunda) ya da bir "spurious wakeup" oluştuğunda, thread uyanır ve mutex yeniden edinilir.
        

- Uyanan ve kilidi edinen thread'in koşulun gerçekleşip gerçekleşmediğini kontrol etmesi ve eğer bir spurious wakeup söz konusu ise tekrar bekleme durumuna geçmesi gerekir.
  Yani aslında,
  1. değişiklik yapılmış mı, 
  2. wait funclarından birini çağırırız, 
  3. condition_variable nesnesinin notify funcı çağrıldığında spurious wakeup var mı yok mu diye bakılıyor
  spurious wakeup ise tekrar thread uyanıyor ve mutex yeniden elde ediliyor.

- İkinci seçenek olarak bekleyen thread.Yukarıdaki işlemlerin hepsini birlikte yapıyor.
	-- Bu işlemlerin hepsini sarmalayan wait fonksiyonlarının bir predicate alan overload'larından birini çağırır.
       Çok daha kolay bir yol.
	   3 aşamayı birlikte yapıyor.
	   Kilidi ediniyoruz ama wait funclarının predicate isteyen overloadları var.wait, wait_for wait_untill hepsinin var.
	   Tüketici taraf olarak unique_lock ile kilidi edindik, wait funcını çağırdık.
	   Bu function 3 aşamayı beraber yapıyor.


std::condition_variable sınıfı yalnızca std::unique_lock std::mutex ile kullanılabilir. Bu şekilde kullanım zorunluluğu bazı platformlarda en yüksek verimle çalışmasını sağlar. 
std::condition_variable_any sınıfı ise BasicLockable niteliğini sağlayan herhangi bir nesneyle (örneğin std::shared_lock) çalışabilmesini sağlar.
std::condition_variable sınıfının wait, wait_for, wait_until, notify_one ve notify_all üye fonksiyonları birden fazla thread tarafından eş zamanlı çağrılabilir.

std::condition_variable sınıfının wait üye fonksiyonu, (busy wait) için bir optimizasyon olarak görülebilir. wait fonksiyonunun (ideal olmasa da) gerçekleştirimi şöyle olabilir:

MINIMAL TEMPLATE:
template<typename Pred>
void wait(std::unique_lock<std::mutex>& lk, Predicate pred)
{
	while(!pred()) {
		lk.unlock(); // Burada mutex serbest bırakıldı ve waiti çağıran threadin çalışması durduruldu
			     // Burada kilit serbest bırakıldıki diğer taraf çalışsın

		lk.lock();    
	}
}

condition_variable sınıfı condition_variable.h başlık dosyasında.

----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------

ÖR

#include <mutex>
#include <condition_variable>

bool ready_flag{};	// BU CONDITION. HER 2 TARAFTA BUNU KULLANACAK. 
					// GÖREVİ TAMAMLAYAN BU BAYRAĞI SET EDECEK. TÜKETİCİDE BU BAYRAĞIN TRUE OLUP OLMADIĞINA BAKACAK.

int data{};			// BURASI TASKİ TEMSİL EDİYOR

std::mutex mtx;

std::condition_variable cv; // BUDA CONDİTİONAL VARİABLE


void producer()
{
	std::cout << "Producer is producing the data\n";
	
	{
	
		std::lock_guard lock{mtx};
		data = 23894;	
		ready_flag = true;
		// cv.notify_one(); // Burada kilit edinilmişken çağırmak iyi fikir değil.
							// Çünkü diğer taraf kilidi almak için daha fazla bekleyebilir.
							// Bu sebeple alt tarafta notify_one yapmakta fayda var.


	} // Buradan sonra kilit açılıyor çünkü lock guardın hayatı bitince edindiği mutexi serbest bırakıyor.
	
	cv.notify_one();  // Burada neden func call yapılacağını yuakrıda yazdım  
  					  // notify ettiğimizde kilit artık bizde değil.Burada diğer threadlere haber vermiş olduk


}

//Consumer producerın işini yapmasını bekliyor.
void consumer()
{
	{
		std::unique_lock ulock{mtx}; // Beklemeye girmeden önce unique lock ile mutex elde edilir.
		
		cv.wait(ulock, []{return ready_flag;}); // AŞAĞIYI OKU !!!
		// Burada 3 aşamayı kendimizde yapabiliriz yada en iyisi predicate isteyen overload kullanmak.
		// Burada condition ready_flag. O zaman predicate bool döndüren func demek.
		// Burada öncelikle waite argüman olarak geçilen predicate ready flagin true olup olmadığına bakacak
		// wait funcından sonra artık uyuyor. Thread notification geldiğinde uyanır.
		// Eğer bir spurious wakeup olursa thread her uyandığında lambda callable çağrılacak.
		// callable false değer dönerse spurious wakeup olduğu anlaşılır ve tekrar uykuya dalar.
		// Buradan çıkmak için ready flag gerçekten set edilecek.

		// Çağrılan wait aynı zamanda, kilidi henüz bırakmadan doğrudan ready flag değişkeninin set edilip
		// edilmediğine uykuya dalmadan bakıyor. Çünkü zaten wait çağrıldığında iş işten geçmişse, notification verilmişse
		// eğer ona bakmazsa sürekli uyku halinde kalır. Birdaha notification gönderilmeyebilir çünkü.

		// Bu noktada artık ne işim varsa devam edebilirim
	}

	// Burada artık procuder veriyi üretmiş, consumer artık bundan emin yoluan devam edecek.
	std::cout << "data = " << data << "\n";
}


int main()
{
	std::thread t1{producer};
	std::thread t2{consumer};

	t1.join();
	t2.join(); 
}


MOLA

----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------

ÖR: Thread safe basit stack uygulması

#include <mutex>
#include <vector>
#include <condition_variable>
#include <thread>
#include <iostream>

class IStack {
public:
	IStack() {};
	IStack(const IStack&) = delete;
	IStack& operator=(const IStack&) = delete;
	
	int pop()
	{
		std::unique_lock lock(m_);  // unique lock kullanmak zorundayız
		m_cv.wait(lock, [this]() {return !m_vec.empty(); }); // wait ten çıkıldığında kilit edinilmiş olur
		int val = m_vec.back();
		m_vec.pop_back();
		return val;
	}

	void push(int x)
	{
		std::scoped_lock lock(m_);	// kilidi edindi
		m_vec.push_back(x);		// işini yaptı
		m_cv.notify_one();   // notify yı çağırdı. şimdi pop koduna bak
	}
private:
	std::vector<int> m_vec;
	mutable std::mutex m_;
	mutable std::condition_variable m_cv;
};

constexpr int n = 1000;
IStack s;

void producer() 
{
	for (int i = 0; i < n; ++i)
		s.push(2 * i + 1);
}

void consumer()
{
	for (int i = 0; i < n; ++i)
		std::cout << s.pop() << '\n';
}

int main()
{
	std::thread th1(producer);
	std::thread th2(consumer);

	th1.join();
	th2.join();
}

----------------------------------------------------------------------------------------------------------------------

cppreference örneği

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;	// mutex
std::condition_variable cv;  // conditional variable
std::string data;   // data
bool ready = false; //  bu ve alttaki conditionlarımız. 2 adet condition var.
bool processed = false;

void worker_thread()
{
	// Wait until main() sends data
	std::unique_lock lk(m);	// 2. burada kilit alındı
	cv.wait(lk, [] {return ready; }); // 3.  beklemeye girdi. ready değişkeni true olana kadar bekleyecek
					  // ready nin true olması için diğer tarafın işini yapıp ready yi true hale getirmesi lazım
					  // wait funcı unlock ve lock funclarını çağırıyordu. Yukarıda basit kodu vardı

	// after the wait, we own the lock.
	std::cout << "Worker thread is processing data\n"; // 9. notify one çağrılınca 8 adımdan buraya geçtik.
	data += " after processing"; // 10. dataya bu yazı eklendi.

	// Send data back to main()
	processed = true; // 11. bu conditon değeri true hale geldi
	std::cout << "Worker thread signals data processing completed\n"; // 12.

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lk.unlock(); // 13. kilidi unlock etti ve 
	cv.notify_one(); // 14. notify one çağrıldı
}

int main()
{
	std::thread worker(worker_thread); // 1. ilk olarak worker thread funcı çalıştırdı

	data = "Example data ";  // 4.
	// send data to the worker thread
	{
		std::lock_guard lk(m); // 5.
		ready = true; // 6. Threadde beklenen o ready değerinin true olması işi yapıldı
			      // burada main funcı producer gibi çalışıyor.

		std::cout << "main() signals data ready for processing\n"; // 7.
	
	} // burada blok sona erince kilit serbest bırakılmış oldu

	cv.notify_one(); // 8. bu func çağrılınca func içindeki blok uyandı. TAKİP ET !

	// wait for the worker
	{
		std::unique_lock lk(m); // 15. // bu sefer burada beklemeye girdi.
		cv.wait(lk, [] {return processed; }); // 16. process değişkeni true hale gelince uyandı
	}
	std::cout << "Back in main(), data = " << data << '\n'; // 17.

	worker.join(); // 18.
}

----------------------------------------------------------------------------------------------------------------------

Josuttis in kitabında hata var. Ona bakıldı.

Release lock olayına bak.
release ile unlock farkına bak.cppreference ta yazıyor.

cppreference tan bakıldı
release, unique_lock gibi bir sınıf için edinilmiş mutexi, unlock functionunu çağırmadan bırakıyor.
unlock ise herhangibir mutexi bırakmıyor, sadece unlock işlemini yapıyor yani kilidi açıyor.

----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>

std::condition_variable cv;
std::mutex cv_m;
// mutexin kullanılma sebepleri
// 1 - i ye erişimi senkronize etmek.
// 2 - std::cerr e erişimi senkronize etmek
// 3 - conditional variable cv için

int i = 0;

void waits()
{
	std::unique_lock<std::mutex> lk(cv_m); // signal ile aynı mutex edinildi
	std::cerr << "Waiting...\n"; // mutexi edinen her thread burada waiting yazacak
	cv.wait(lk,[]{return i == 1;}); // buraya gelince beklemeye başladılar.
									// i, 1 olduğunda çıkacak beklemeden.

	std::cerr << "...finished waiting. i == 1\n";
}

void signals()
{
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1 saniye beklettiğinde i nin değeri halen 0.
															// bu durumda waits funcları beklemeye devam ediyor.

	{
		std::lock_guard<std::mutex> lk(cv_m);  // mutex acquire edildi
		std::cerr << "Notifying...\n"; // cerr unique buffered. flash edilme mecburiyeti yok.
	}

	cv.notify_all();  // burada herkese notify etti.

	std::this_thread::sleep_for(std::chrono::seconds(1));   // ikinci kez bekletiliyor ve aşağıda i = 1 yapıldı

	{
		std::lock_guard<std::mutex> lk(cv_m);
		i = 1;
		std::cerr << "Notifying again...\n";
	}

	cv.notify_all(); // burada notify edince artık wait funclarıda i nin değeri 1 olduğundan beklemeden çıkar.
}

int main()
{
	std::thread t1(waits), t2(waits), t3(waits), t4(signals); 

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}

*/
