/*

Biraz Gruptan sorulan sorulardan bahsedildi.

SOLID İLKELERİ
--------------

Single Responsibility Principle(SRP)
------------------------------------
Robert C. Martin diyorki,
Genelde yanlış anlaşılıyor.Bunun sebebi isminin uygun olmaması.İyi seçilmemiş.
İlkenin ismini duyan programcılar her modülün tek birşey yapması gerektiği anlamını çıkarıyorlar.
Böyle bir ilke var ama o ilke bu ilke değil.O ilke functionlara yönelik. Bir function tek bir şey yapmalı. 
C de hoca anlattı. Aslında o da soliddeki SRP değil. O kod seviyesinde refactoring yaparken
büyük funcları küçük funclara bölerken kullandığmız seviye.Bir kod bloğu açıklamaya ihtiyaç duyuyorsa
onu ayrı bir func olarak yazmalıyız.

BU DURUMDA SRP NEDİR?
Tarihsel olarak SRP şöyle tanımlanıyor, bir modül sadece ve sadece tek bir nedenden ötürü değişmeli.
Yazılımsal sistemler kulanıcıları/user ve paydaşları/stakeholderı tatmin etmek için değişirler.
İlkede kastedilen değşme nedeni bu kullanıcılar ve paydaşlardır.Tek neden dediği user veya stakeholder.

Aynı ilkeyi şu şekilde de ifade edebiliriz.
Bir modül sadece bir kullanıcıya karşı sorumluluk taşımalıdır.Burada user/stakeholder doğru kelimeler değil.
Sistemin aynı şekilde değişmesini talep eden birden fazla kullanıcı yada paydaş olabilir.Yani sistemde bir değişiklik gerektiğinde
o değişikliği birden fazla kullanıcı yada stakeholder talep ediyor olabilir. 
Burada kastedilen değişikliği talep eden bir veya daha fazla kişinin oluşturduğu grup, bu grubada aktör diyor.
Yani Sistemle ektileşime giren aktör.Aktör illaha kullanıcı olmak zorunda değil. Başka bir program veya sistem olabilir.

SRP şu şelkilde ifade edilir. 
Bir modül sadece tek bir aktöre karşı sorumluluk taşımalıdır.
O aktörün talebi değiştiğinde değişecek yani. Aktör  = User + Stakeholders
Birden fazla farklı aktöre göre değişmeyecek.

Burada modül derken neler kastediliyor.En kısa tanımla, modül kaynak dosyadır (C++/Java içi)
Diğer programlama dillerinde de modül functionların ve veri yapılarının oluşturduğu cohesive bir kümedir.
Cohesive SRP yi işaret eder.

Bir sınıf tek bir iş yapmalı değil ama bir function tek bir iş yapmalı.
Bir sınıf tek bir nedenden değişmeli derken aktör sınıfın yada modülün hizmet verdiği,
kime karşı sorumluysa modül ondan gelen değişme talebi.

Burada yanlış anlaşılan, her sınıfa tek bir sorumluluk vereyim diye sadece bir funcitona sahip 10larca
sınıf oluşturuyor programcı. Yani bir functionu yazması gerektiği şekilde sınıfı yazıyor.
Bu hatalı.

--------------------------------------------------------------------------------------------------------------------------------------------

Open Close Princible(OCP)
-------------------------
Modüller değişikliğe karşı kapalı ama genişletmeye karşı açık olmalıdır.

Bir değişiklik implemente etmek için varolan kodu değiştirmek yerine yeni kod yazmak.
Varolanı değiştirme, kod ekle. C den bile bilinen bir ilke.

Yani sınıf bazında düşünürsek,  sınıflar extensiona/genişlemeye açık değişime kapalı olmalı.

Bunu nasıl kullanabiliriz?
Değişikliğin nerelerden geleceğini öngörebilmeliyiz

Özetle eski kodların değiştirilmeden yeni taleplerin ekleme yoluyla gerçekleştirmesi.

1:29

--------------------------------------------------------------------------------------------------------------------------------------------

Liskov Substitution Principle(LSP)
----------------------------------
Polimorfizmin güvenli kullanımına ilişkin bu. 
Subsclases for be substitutable for their ....
Taban sınıf nesnesinin kullanıldığı yerde onun yerine türemiş sınıf nesneside kullanılabilir.

Substitution derken birtakım garantiler olması anlamında kullanılıyor.

--------------------------------------------------------------------------------------------------------------------------------------------

Interface Seggregation Principle(ISP)
-------------------------------------
Client kodlar kullanmadıkları arayüzlere bağımlı olmaya zorlanmamalıdır.
KUllanılmayan kodları uygulamaya zorlamamamız gerekiyor.

Özel ince ayarı yapılmış interfaceler kullanılmalı. Onları kullanmayacakları interfacelere implemente
etmeye zorlamayın.

Oluşturduğumuz arayüxün genel olduğunu düşünüyorsak clientlar bunları kullanmaz diyorsak
bunları bölmek daha mnantıklı. Herbiri ayrı metod gruplarını içerse daha iyi olur.
Daha cohesive interfaceler kullanın diyor bu ilke.

--------------------------------------------------------------------------------------------------------------------------------------------

Dependancy Inversion Principle(DIP)
-----------------------------------


*/
