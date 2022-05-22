/*

ATOMİK TÜRLER
-------------
Atomik işlem, bölünemeyen bir işlem demek. Öyle bir işlemki birden fazla parçayla yapılıyor olsa dahi
o parçaların arasına başka threadlerden başka işlemler giremiyor demek.

Ör:
++x 

read 

modify

write işlemleri yapılarak yukarıdaki işlem gerçekleşiyor

Write tan önce başka bir thread x i okuyabilir. Bu işlemler atomik olarak yapılmazsa ve böyle
değişkenler birden fazla threadde paylaşılan değişkenler ise bir data race oluşturabilir ve bu da UB demektir.

Atomik işlem olması durumda bir garanti var.
Bu işlemin atomik olması demek, bir başka threadin ya bu işlem tamamalanmadan
bellekte x i görmesi ya da bu işlem tamamlandıktan sonra x in değişmiş değerini görmek
demek. Ara bir state te x i görmüyor. Bu garanti veriliyor.

Bu özelliğe sahip değişkenler/atomik değişkenler torn read tornw write denilen disk durumlarına sahip değiller.

C++ library atomik işlemlerin yapılabileceği sınıf türlerine sahipler.2 sınıf var aslında biri template.
Bu templatten oluşturulan explicit ve partial specializationlar var.

Yani biz int türden değişken kullanmak yerine atomik özelliğe sahip int i sarmalayan bir atomik değişken
kullandığımızda o değişken üzerinde yapılan bazı işlemler atomik olarak gerçekleşecek.

NOT!!!!
Tipik yanlış anlama
2 ayrı işlem atomikse, bu iki işlemin oluşturduğu bir ifadedeki işlemlerin toplamı atomik olmak zorunda değil.

++x + y++  --> x te y de atomik özelliğe sahip değişkenler olsa, bu işlemin tamamı atomik değil.
x++ bundan sonra x i kullansak, bu işlemin toplamının atomik olma zorunluluğu yok.Araya işlem girebilir.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

BIR DEĞIŞKENIN ATOMIK OLMASI NE DEMEK ? 
Bir değişken atomikse, ++ işlemide o atomik değişken üzerinde yapılabilecek atomik bir işlemse 50 tane
thread oluştursak, 50 threadde 50 tane task çalıştırsam, bu tasklerin hepsi x değişkenini 1 veya daha fazla artırsa
bunların artırmalarının hepsi gerçekleşecek anlamında.

İşlemlerin atomik olup olmaması işlemciye bağlı daha çok.Dolayısı ile işlemin atomik olması garantisi 
ile lock kullanma garantisi aynı şey değil. Belli bir tür için belli bir işlem atomik olabilir.
Fakat geriplanda bu üretilen kodda cpu nun bir instructionu ile yapılabilir yada bunu implemente eden kod
arka planda bir kilit sistemi kullanıyor olabilir.

Yani atomik bir tür üstünde yapılan atomik bir işlem lock free olmak zorunda değil.İşlemin kendisini
implemente etmek için bir kilit sistemi kullanmıyor olabilir. Bu tamamen atomik olarak kullanılacak türün
hangi tipten veriyi sarmaladığı ile ve hedef işlemcinin ne olduğuyla ilgili. Standart library de bunu test
eden bir function var. Bunun ismi lock_free.

C++ IN TARTIŞMASIZ EN KARMAŞIK KONUSU BU KONU
Burada birinci kullanım seviyesi productionda çalışan programcıya ilişkin.Standart programcı.
İkinci seviyede ise low level interface var.Yani lock free denen programlama modeli.
Lock free, program runtimeda birden fazla thread kullanıyor olsada hiçbiri bloke olmayacak
her an bir thread ilerliyor olacak.
Bir sonraki aşama wait free programming, hiçbir thread beklemeyecek.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

ATOMIK TÜRLERI VE ATOMIK IŞLEMLERI NEDEN KULLANIYORUZ?
Atomik işlemler non divisible garantisi veriyor. Atomik işlemler tek alternatif değil.
Kilit sistemleri yani mutex kullanımı her zaman bir alternatif.Birden fazla tema var.
Temalardan biri verimlilik. Verimlilikte bedavaya gelmiyor. Yani mutex kullandığım heryerde 
büyük beceryile atomik türleri kullandım ve program daha verimli çalışıyor kesin diyemeyiz. Olabilir
olmayadabilir. Bu konuda verilen tavsiyeler var. 

Birinci Tavsiye, 
Eğer kilit sistemleri işimizi görüyorsa
herhangibir bottleneck yoksa veya herhangibir data race, deadlock, livelock problemleri riski yüksekse,
yani programın genel yapısı burada kullanılan mutexlerle ilgili bu garantiyi verme konusunda tereddütteyse
atomik tercihi kullanılabilir. Yani seçeneklerden biri doğrudan efficiency ile ilgili değil mutex kullanmanın
olası dezavantajlarından kaçınmak için. Bir nedende efficiency.

İkinci Tavsiye,
Atomik kullanıyoruz diye daha hızlı olacak dememeliyiz her zaman profile etmeliyiz.

Üçüncü Tavsiye
Platforma yönelik kestirimlerden kaynaklı atomikler kullanmayın deniyor.Bug oluşturma riskini çok artırıyor.
Yani belirli bier mimari için kod yazıyoruz, special instructionlar var. Bunların faydalarından faydalanmak için
atomik türlerin low level interfaceini kullanıyorum.

Low level derken, atomik türler üstünde yapılan işlemler belirli garantileri veriyorlar, dil bu garantileri
dokümante etmiş. Burada belirli garanti seviyeleri var. Sequencial consistency semantic gibi. Release acquire semantic gibi.
Yada release consume modeli gibi. Bunlar en yüksek seviyede interfacete görülen funcların 
default argümanlarla kullanılmış hali. Default argümanlarla kullanılmış halide her zaman sequencial
consistency. Daha fazla garantinin sağlanması programlamanın daha kolay, davranışın daha iyi kestirilebilir
olmasını sağlarken tabi ki işlemciyede bağlı ama ilave maliyet ekliyor.

Low level interface e geçmemizin sebebi aslında default argümanların kullanıldığı daha yüksek interface teki
verilen garantilerin belirli bir sistemde ilave maliyet getirmesi. Bir şey ilave maliyet getiriyorsa doğrudan
hemen ondan kaçınmak çok doğru olmayabilir.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

ATOMIC GITHUB NOTLARI !!!!

- Thread'ler arasında veriyi paylaşırken "data race" oluşmaması için genellikle bir senkronizasyon mekanizması gereklidir.
  (örneğin mutex kullanarak senkronizasyonu sağlamak).
- Veriye senkronize edilmiş erişim sağlamanın başka bir yolu da atomik türleri kullanmaktır.
- Bölünemeyecek (indivisible) bir işleme "atomik işlem" denir. Atomik bir işlem arasına başka bir işlem giremez.
  Örneğin arttırma işlemi atomik ise bu işleme tabi tutulan bir değişken diğer thread'ler tarafından ya işlemden önceki ya da 
  işlemden sonraki değeri ile görülebilir. İşlemcilerin çoğunda atomik bellek işlemleri özel makine komutları ile gerçekleştirilir.
- atomik bellek işlemleri "torn read" ya da "torn write" durumu oluşturmazlar.
- C++ standart kütüphanesi atomik bellek işlemleri için aşağıdaki atomik türleri sunmaktadır:
	std::atomic_flag
	std::atomic<>
- Bu türler donanımın sağladığı atomik operasyonlar için uniform bir arayüz sunarlar.


Atomik bir tür aşağıdaki garantileri sağlar:
- atomicity (atomiklik - bölünmezlik)
- ordering (bellek işlemlerinin sıralanması - önceliği sonralığı)
- visibility (bellek işlemlerinin sonuçlarının diğer thread'lerden görülmesi) Sıralı işlem garantisi 
  bellek işlemlerinin diğer thread'lerde ne şekilde görülebileceği garantisidir. 
  Atomik türler birbirinden bellek işlemlerinin sıralanması konusunda farklı garantiler sunarlar. 
  Varsayılan düzen "sequentially consistency"'dir. (daha sonra değinilecek)


atomic değişkenlerle yapılan işlemler atomik olma garantisini taşırlar. 
Yani bu işlemler tamamlanıncaya kadar bir başka thread aynı değişken üzerinde işlem yapamaz. 
Atomic bir değişkenin değerinin (atomik olarak) bir arttırıldığını (increment) düşünelim. 
Bu işlem aslında 3 farklı işleme karşılık gelir: oku - değiştir - yaz (read - modify - write). 
Eğer değişkenimizin atomik olma garantisi yok ise bu işlemlerin arasına başka thread'lerin 
işlemleri girebilir. Değişkenimiz atomik ise diğer thread'ler değişkenimizi ya değiştirilmeden 
önceki değeri ile ya da değiştirildikten sonraki değeriyle görebilirler, ara bir değerde göremezler. 
Bir başka deyişle atomik değişkenler için torn-read ya da torn-write olamaz.


std::atomic_flag
- std::atomic_flag türü sadece temel işlemler sunar.
- Bu türden bir değişken (flag) yalnızca iki durumda olabilir. Ya set edilmiş durumda (true) ya da 
  clear edilmiş durumda (yani false) 
- bu tür 2 operasyon sunar:
	-- test_and_set: state'i true yaparken set işleminden önceki değeri sorgular (get eder).
	-- clear: state' i false değere çekiyor.
- varsayılan kurucu işlev (default ctor) nesneyi belirlenmemiş bir değerle başlatır. 
  C++20 standartları ile bu durum değişti. Default ctor artık false değeri ile başlatıyor.
- Kopyalanamayan ve taşınamayan bir türdür.
- Derleyiciye bağlı olan ATOMIC_FLAG_INIT makrosu bu türden bir değişkene ilk değer vermek için kullanılır:


std::atomic_flag f = ATOMIC_FLAG_INIT;


- Bir kilit mekanizmasının kullanılmadığı garanti edilmektedir.
- Diğer atomik türlerde olan is_lock_free üye fonksiyonuna sahip değildir (zaten lock-free olmak zorunda).
- std::atomic_flag, spinlock mutex gibi daha yüksek seviyeli senkronizsayon mekanizmalarında araç olarak kullanılır.



std::atomic
std::atomic bir sınıf şablonudur. Bu sınıf şablonundan elde edilen türler atomik operasyonlar sunar.

TEMPLATE:
template <typename T>
struct atomic;

- Tam sayı türleri ve pointer türleri için bu sınıfın _"partial specialization"_ları var.
- T türünden bir nesne için atomik operasyonlar sağlıyor.
- Tüm temel türler (fundamental types) için standart kütüphane tarafından "full specialization" sağlanmış durumdda.
- Template argümanı olarak kullanılacak türün "trivially copyable" ve "bitwise equality comparable" olması gerekiyor.
- Kilit sistemi kullanılmama (lock-free) olma garantisi verilmiyor. Ancak platformların çoğunda temel türler olan 
  template argümanları söz konusu olduğunda bir kilit sistemi kullanılmadan (lock-free) gerçekleştiriliyor.
- Kopyalama ya da taşıma yoluyla nesne oluşturulamıyor. (not copy-constructible - not move constructible)
- Atama yapılabilir (assignable) bir tür. Ancak atama operatörü referans değil değer döndürüyor.
- Varsayılan bellek düzeni (memory order) std::memory_order_seq_cst.


-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

MEMORY MODEL
-----------
Bİzim programımızın(özellikle multithread çalışacakların) bellekteki read write işlemleriyle ve 
doğal olarak paylaşılan değişkenlerle ilişkisini formal olarak tanımlayan bir döküman.
Bir çok sorunun cevabı formal olarak memory model ile veriliyor.Asıl büyük devrim C++ a memory modelin eklenmesi.

Modern C++ öncesindeki durumu standartlar implementasyona göre değil C++ Abstract machine e göre belirlenen
soyut yapıya göre kuralları tanımlıyordu. Daha önceki C++ tanımı tamamen sanki tüm programlar single thread 
çalışacak gibi düzenlenmişti.

Multithread programlama library ile gerçekleştiriliyordu, dilin araçlarıyla değil.
Dilin standartları multithread programlara ilişkin herhangibir kural seti söylemiyordu.
Thread kavramı yoktu.

C++11 bir Memory Model getiriyor. DOlayısıyla birden fazla thread içeren bir programın runtimedaki
davranışını, bu memory model ve getirdiği kuralları doğrultusunda davranışını kestirebiliyoruz.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

GITHUB NOTLARI
--------------
MEMORY MODEL NEDIR?
- Bilgisayar programının belleğe erişiminde izin verilebilen semantik yapıyı tanımlar.
  Bellekten bir okuma yapıldığında hangi değerlerin okunması beklenebilir. Hangi durumlarda UB oluşabilir.
  Bunları kapsıyor. Bellek programa ve programda kullanılan threadlere nasıl görünüyor.Bu uygun cümle
  Threadlerin eş zamanlı çalışıyor olması değişkenleri eşzamanlı olarak aynı değerde göreceğimiz garantisi değil.
  Bu garanti ne zaman var ne zaman yok hangi ihtimaller sözkonusu? Bunları Memory Model tanımlıyor.
- Bellekten bir okuma yapıldığında hangi değer /değerler okunabilir? 
  (hangi değerlerin okunması beklenebilir)
- Hangi durumlarda tanımsız davranış oluşabilir?
- Memory model, multithread progranmlar için kritik olan bir bileşendir.
- Thread'ler bellek ve paylaşılan veril(ler) ile nasıl etkileşime giriyor? 
  Bellek programa ve programda kullanılan thread'lere nasıl görünüyor?
- Multi-thread programların yürütülmesine ilşkin kuralların belirlenebilmesi için 
  programlama dilinin bir memory model oluşturulması gerekiyor.


Thread'ler, programın çalışma zamanında paylaşılan verileri farklı değerlerde görebilir. Bir memory model oluşturmadan 
derleyici optimizasyonları ve donanım tarafında yapılan optimizasyonlar sorunlar oluşturabilir. 
thread'lerin paylaşılan değişkenlerde yapılan değişiklikleri anında gözlemleyebilmeleri verim (efficiency) açısından yüksek bir maliyet oluşturur.


KAYNAK KODUMUZDAKI IŞLEMLER
---------------------------
- Derleyici tarafından
- CPU tarafından;
- Bellek (caching) tarafından yeniden sıralanabilir


Memory model hangi durumlarda yeniden sıralamaya (reordering) izin verilip verilmediğini belirler.
Birden fazla thread hangi durumlarda paylaşılan değişkenlere erişebilir? 
Bir thread tarafından bir değişkene yapılan atama eş zamanlı çalışan thread'ler tarafından ne zaman görünür olacak? 
Programcılar multi thread programların nasıl çalışacağını (belirli garantiler altında) kestirebilmeliler. 
memory model buna yardımcı oluyor.

- C++11 öncesinde C++ dilinden formal bir bir memory model yoktu. 
  C++ soyut makinesi (abstract machine) single-thread olarak tasarlanmıştı. 
  java memory model 1995 yılında oluşturuldu. Yaygın kullanılan programlama dilleri içinde bu açıdan 
  java bir ilk. pthreads kütüphanesi ilk olarak 1995 yılında, bir memory modele dayanmadan geliştirildi. 
  Posix memory model oluşturmuyor. Tanımlar kesin (precise) değil. Yani, "program doğru mu" sorusunun formal bir cevabı yok.
  İlk olarak C++ dilinde java'nın memory modelinin kullanılması düşünüldü, ancak bunun uygun olmadığı görüldü (fazla kısıtılayıcı). 
  Java memory modeli belirli veri türlerinin atomik olmasını gerektiriyor. Java memory model C++ için çok pahalı. 
  Bir thread library oluşturabilemek için derleyici üstünde bazı kısıtlamaların olması gerekiyor.
  Bunun için de bir memory model'e ihtiyaç duyuluyor.

  
Bir programın çalışma zamanında nasıl davranacağını kestirebilmemiz için şunları bilebilmemiz gerekir:
- Programdaki (farklı thread'ler tarafından gerçekleştirilebilecek) işlemlerin (operasyonların) 
  hangi sırayla yürütüleceği (ordering)
- Programdaki bir işlemin/işlemlerin sonuçlarının (başka thread'ler tarafından yürütülebilecek) 
  diğer bir işlem/işlemler yapılmadan görülür olup olmadığı (visibility)

	x = 4;		b = y;
	y = x;		c = x;
	a = y;		z = y;

	x = 4 işlemi yapıldı ve bir side effecti var. eşzamanlı çalışan threadlerde c = x; ypılınca
	x in değişmiş değerini mi yoksa değişemiş değerini mi kullanacağız.
	Buna yönelik kurallar


Tek bir thread'in söz konusu olduğu programlarda yukarıdaki soruların cevabı sezgisel olarak verilebilir.
Birden fazla thread'in söz konusu olduğu programlarda ise bu durum çok daha karmaşıktır.



SEQUENCIAL CONSISTENCY
----------------------
Sırada tutarlılık. Programın çalışması sırasında farklı threadlerdeki farklı işlemler tek bir threadde sıralı
çalışıyormuş gibi olmak zorunda.Ama sırası aynı olmak zorunda değil. 45.33 resim var. 
sequencial consistency.png

Resimdeki örneğe benzer örnek verildi.

Derleyici burada reordering yapabilir. İşlemlerin sırasını değiştirdi ama bu değişiklik sonucu
yürütülen bu kodlar sırası değişmeden yürütüldüğünde oluşan sonuçların aynısı oluşacak.



HAPPENS-BEFORE ILIŞKISI !!!!!!!!!!!!!!!!!!!!
-----------------------
Bu standartların kulladığı bir terim. Happens before demek, zamansal olarak bir işlem bir işlemden
önce yapılıyor anlamına gelmiyor. Mesela fiziksel süre sözkonusu olduğunda bir işlem diğerinden
daha kısa sürede yapılıyordur ama bu happens before olmak zorunda değil. Hatta tersi bile olabilir.
A ve B arasında happens before ilişkisi var diyelim, ama fiziksel olarak baktığımızda A, B den daha
önce yapılmış olma zorunda da olmayabilir.


KRİTİK NOKTALAR !!!
---------------
A ve B iki operasyon olsun. Bu operasyonlar aynı thread'de gerçekleştirilen operasyonlar olabildiği gibi 
farklı thread'lerde gerçekleştirilen operasyonlar da olabilir. 

Eğer A operasyonunun etkileri B operasyonunu yürütecek thread'de 
B operasyonu yürütüldüğünde görülür durumda ise, bu A HAPPENS BEFORE B demektir.
Böyle bir garanti söz konusudur.

A HAPPENS BEFORE B demek, A operasyonu yapılmış, onun etkileri tamamlanmış, B yürütüldüğünde buna güvenerek
B operasyonları yapılabilir demek. Bu da şu demek, 2 nokta arasında A HAPPENS BEFORE B ilişkisi varsa
aynı yada farklı threadde, biz B nin yürütülmesi durumunda A daki oluşan işlemlerin side effectlerini
kullanabiliriz demek.

Happens-before ilişkisi "zamansal olarak daha önce gerçekleşme" ilişkisine göre çok daha güçlü bir durumdur !!!!!!!!!!
Yani happens-before in time. happens-before in time olması happens before ilişkisi olduğunu garanti etmiyor !!!!!
Happens before ilişkisinin olması için dilin tanımladığı bazı durumlar var.
Dil diyorki belirli koşullar olursa A ile B arasında happens before ilişkisi vardır.
Bir kısmı senkronizasyon ile ilgili diğerleri ilse biraz daha yan durumlar.

Eğer A operasyonu B operasyonuna göre zamansal açıdan daha önce gerçekleştiriliyor ise bu A'nın etkilerinin 
B'yi yürütecek thread'de B yürütülmeden önce görülür olma garantisi değildir. caching, store buffer vs gibi 
mekanizmalar operasyonun etkilerinin diğer thread'lerde görülmesini geciktirebilir.

Atomik değişkenler doğrudan bir happens before ilişkisi oluşturmuyor.Atomik değişkenlerin bazıları
kullanımları happens before ilişkisi oluşturuyor. Yani bir değişkenin atomik olması demek,
onun tanımladığı atomik işlemlerin atomik olarak yapılması demek. Ama doğrudan atomik kullandığımızda
bir happens before ilişkisi garanti değilmiyor.



SEQUENCED BEFORE ILIŞKISI !!!!!!!!!!!!!!!!!!!!!!!
-------------------------
Sequenced-before ilişkisi aynı thread'deki işlemlere ilişkindir.
Aynı threaddeki işlemler(özel durumlar hariç) programdaki sıraya göre yapılmak zorunda.

Sequenced-before ilişkisi geçişkendir (transitive) 
A sequenced before B doğru ise 
B sequenced before C doğru ise 
A sequenced before C doğrudur.

x = 1; 		// A  önce bu
y = 2; 		// B  sonra bu
z = x + 1;	// C  en sonda bu



Aşağıdaki kodu ele alalım:

y = a * x + b; // (y = ((a * x) + b);
Bu ifadede 3 işlem var: çarpma toplama ve atama. 
Burada
çarpma işlemi sequenced before toplama işlemi
toplama işlemi sequenced before atama işlemi
dolayısıyla
atama işlemi sequenced before toplama işlemi

------------------------------
------------------------------
Eğer bir thread için
A sequenced before B doğru ise
A happens before B doğrudur.
------------------------------
------------------------------

Bu şu anlama geliyor:
Sequenced-before ilişkisi aynı zamanda thread içindeki (intra-thread) happens before ilişkisine karşılık geliyor.
Happens-before ilişkisi zamana bağlı değildir, görünürlüğe (visibility) bağlıdır.

A happens before B ise, bu A'nın B'de önce yapılması anlamına gelmez (böyle bir garanti yoktur).
A, B'den önce yapılmış ise A happens-before B olmak değildir.
happens-before ilişkisi acquire-release semantiği ile gerçekleştirilebilir. (Daha sonra ele alacağım)
Happens-Before Does Not Imply Happening Before!


Aşağıdaki koda bakalım:

#include <iostream>

int is_ready{ 0 };
int value{0};

void producer()
{
	value = 42;                         // (1)
	is_ready = 1;                       // (2)
}

void consumer()
{
	if (is_ready)                       // (3) Burada okunan değerin 1 olduğunu düşünelim
		std::cout << value;             // (4)
}


Buradaki fonksiyonlar iki ayrı thread tarafından çalıştırılıyor olsun. 
Değişkenlere yapılan atamalar (stores) ve değişkenlerden yapılan okumalar (loads) atomik olsun. 
Programın çalışma zamanında consumer thread'inin (3) noktasına geldiğini ve is_ready değişkeninin okunan değerinin 
1 olduğunu düşünelim. Bu değer producer thread'inde (2) noktasında is_ready değişkenine atanan değer.
(2) (3)'ten önce olmuş olmalı. Ama bu (2) ve (3) arasında happens-before ilişkisi olduğu anlamına gelmez.
(2) ve (3) arasında happens-before ilişkisi olmadığı gibi (1) ile (4) arasında da happens-before ilişkisi yoktur. 
Bu yüzden (1) (4) arasındaki bellek işlemleri farklı şekilde sıralanabilir (reordering). 
Derleyici tarafından oluşturulan instruction'lar işlemci ya da bellek tarafından farklı şekilde sıralanabilir (instruction reordering / memory reordering). 
Consumer thread'i (4) noktasına geldiğinde ekrana 0 değeri yazılabilir.


- Inter-thread happens-before ilişkisi(farklı threadler arasındaki happens before ilişkisi)
  farklı thread'ler arasındaki happens-before ilişkisidir.
- A ve B farklı threadler'deki operasyonlar olsun. 
  Eğer A ve B arasında inter-thread-happens_before ilişkisi varsa, A ve B arasında happens-before ilişkisi vardır.
  Yani A'daki operasyonların sonucu B'de görülebilir olmak zorundadır.
- Inter-thread happens-before ilşkisi geçişkendir (transitive)
  A ve B arasında inter-thread happens before ilişkisi varsa,
  B ile C arasında inter-thread happens before ilişkisi varsa,
  A ile C arasında da inter-thread happens before ilişkisi olmak zorunda.
- Inter-thread-happens-before ilişkisinin oluşması için (dil tarafından tanımlanan) bir senkronizasyonun söz konusu olması gerekir.
- A ve B arasında happens-before ilişkisi olsun. Bu durumda
  -- A ve B aynı thread'de olabilir ve aralarında sequenced-before ilişkisi vardır.
  -- A ve B aynı farklı thread'lerde olabilir ve aralarında intra-thread-happens-before ilişkisi vardır.

GITHUB NOTLARI BURADA BITTI


-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------


HAPPENS BEFORE İLİŞKİSİNİN OLMASI İÇİN HANGİ SENARYOLAR SÖZKONUSU
-----------------------------------------------------------------

Synchronizes-With Relationship
------------------------------
Bu ilişki varsa farklı 2 thread arasında, inter-thread happens-before ilişkisini garanti 
altına alabiliyor. Yani inter-thread relationship sadece syncronizes with ile sağlanmak
zorunda değil ama birçok durumda happens before ilişkisini sağlayan synchronizes with ilişkisi.

Synchronizes with ilişkisi aynı zaanda happens before u garanti ediyor.

-------------------------------------------------------------------------------------------------------------------

1) Thread Creation 
------------------
Thread sınıfı türünden bir nesne oluşturup Constructruna workload verirsek
eğer ctorunun çalışması tamamlanmışsa workload olarak çalışan functionun çağrısıyla
bir synchronizasyon yaratmış oluyoruz.


ÖR:

#include <thread>
#include <cassert>

int shared = 0; // Bu değişkenin atomik olmasıda gerekmiyor

void func
{
	// B
	// bu kısımda sharedin 1 olması garanti altında
	assert(shared==1); // buraha hata olmayacak. Bu garanti altında
}

int main()
{
	shared = 1;
	/// A
	std::thread t{func};
	
}

-------------------------------------------------------------------------------------------------------------------

2) Thread join
--------------
Thread funcının çalışmasından sonra join funcının çalışmasyla beraber bir senkronizasyon
oluşturmuş oluruz.

#include <thread>
#include <cassert>

int shared = 0;

void func
{
	assert(shared==1); 
	++shared;
}

int main()
{
	shared = 1;
	/// A
	std::thread t{func};
	t.join();

	// Bu noktada shared değişkeninin 2 olması garanti altında
	assert(shared == 1); // hata vermez
}

-------------------------------------------------------------------------------------------------------------------

3) Mutex Unlock/Lock
--------------------
Unlock ile lock arasında bir senkronizasyon ilişkisi var ve bu da bir happens before
ilişkisi doğurur.

std::mutex m;
int x = 0;
int y = 0;

	A									B
thread 1 code                      thread 2 code
--------------					   --------------
m.lock();						   m.lock();
x = 1;			  |------------->  y = x;
m.unlock)---------|				   m.unlock)
			Synchronizes with
	

kiliti thread 1 alsın, x = 1; yapılacak ve sonrasında thread unlock edildiğinde,
unlock ile y = x noktası arasında synchronizes with ilişkisi var. 
Yani B threadi x değerini 1 olarak görmek zorunda.
Yani A happens before B olmak zorunda.

1:27
















































































































































































*/