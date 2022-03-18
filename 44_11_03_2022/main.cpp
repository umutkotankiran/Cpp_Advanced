/*

Policy Based Design da kaldık.
Son yazılan örnek gösterildi tekrar.

NOT : TEMPLATE KODLARDA İSMİ NİTELEMEDEN KULLANIRSAK BU İSİM TABAN SINIFTA DOĞRUDAN
ARANMIYOR.TABAN SINIFTA ARANMASI İÇİN(DAHA ÖNCEDE YAZMIŞTIM)
1 - TABAN SINIFIN İSMİ İLE NİTELENEBİLİR
2 - THIS POİNTERINI KULLANMAK
3 - USING BİLDİRİMİ

-POLICY_5 İ BEN YAZMADIM ÇÜNKÜ POLICY_6 AYNISI AMA DAHA KAPSAMLI.

-POLICY_6 ÖRNEĞİ

-POLICY_7 ÖRNEĞİ

-OBSERVER ÖRNEĞİ
 Bir nesne var, bu nesnede durum değişikliği olduğunda bazı başka sınıflar türünden nesnelerin bu durumdan haberdar olması gerekiyor.
 Haberdar oldukları gibi kendi durumlarınıda o nesnedeki durum değişikliğine göre ayarlamaları gerekiyor.
 Ortada bir nesne var ve bu nesneyle ilgili durum değişikliğine ihtiyaç duyabilecek başka nesneler var.
 
 Durum değişikliği olduğunda statei değişen nesnenin bu değişiklikle ilgili olan nesleri bu durumdan haberdar etmeli.
 Bu iyi bir yöntem.Yüksek düzeyde bir coupling olmadan bunu sağlamak amaç.
 
 Observer kalıbı aşağıdaki durumlarda kullanılıyor.
 Bir soyutlamanın 2 niteliği varsa, 1 bağlı olan nesne biride diğeri. Bunları ayrı nesneler halinde encapsule edilecek ve
 ve bunlar birbirinden bağımsız değişebilsin.
 
 Terminoloji:
 Subject : Durumu değiştiğinde kendi gözlemcilerine bunu haber verek olan sınıf
 Observer : Bunlar gözlemciler
 Concrete Subject : Observera gönderilecek durumu saklar ve oberverlara yollar.
 Concrete observer : Observer interfaceini implemente eder.
 
 
 -WEATHERDATA ÖRNEĞİ
Bir nesne var ve bu nesnede drum değişikliği olduğunda bazı başka sınıflar türündne nesnelerin
bu durumdan haberdar olması gerekiyor ve haberdar oldukları gibi kendi durumlarınıda
o nesnedeki durum değişikliğine göre ayarlamaları gerekiyor.

Mesela data değiştiği için termometre nesnesindeki gösterge değişti gibi.

Durum değişikliği olduğunda statei değişen veya bir event gerçekleştiğinde bu nesnenin
bu değişiklikle ilgili olan nesneleri bu durumdan haberdar etmesi.

Burada yüksek düzeyde bir coupling olmadan kod ekleyerek var olan kodları değiştirmeden
bunun sağlanması, amaç bu.

weatherdata örneğinde değişikliği sınıfların kendisi bildiriyor.
Yaygın olarak kullanılan update interfaceinin şöyle oluşturulması gerekiyor.Gözlemciler belirli değişiklikler için kayıt yaptıracaklar.
Kayıt interfaceinde hangi tür değişiklik olması durumunda update functionu çağrılacağı update funcının intetrfaceinde belirlenecek.
Mesela sınıfın hangi üye funcı çağrılacak gibi bir yapı olabilir.

Bunun dışında birden fazla subjectin gözlenmesi.

VISITOR VE DOUBLE DISPATCH
-DOUBLE DISPATCH ÖRNEĞİ
Double dispatch bir nesnenin türüne değilde birden fazla nesnenin dinamik türüne göre seçilmesi.

-VİSİTOR ÖRNEĞİ
İstediğimiz zaman yeni bir operasyon eklemek istiyoruz.

*/
