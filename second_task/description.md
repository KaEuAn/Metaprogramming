### Многокатегорийная абстрактная фабрика

На вход фабрике подаётся дерево наследований (интерфейс из задания)

Интерфейс получение фабрики:

`using MyFactoryHierarchy = FabricGenerator<3,5, Typelist<Chair, Table, Sofa>, Typelist< WoodenChair, WoodenTable, WoodenSofa>, Typelist< MetalChair, MetalTable, MetalSofa>, Typelist< MetalRichChair, MetalRichTable, MetalRichSofa>, Typelist< MetalPoorChair, MetalPoorTable, MetalPoorSofa> >`
`auto* MyFactory = new MyFactoryHierarchy::GetConcreteFactory<MetalRichChair>::result`

С помощью шаблонной функции Create можно создавать конкретные изделия, которые будут кастоваться сразу к нужному типу с помощью шаблона. 

Фабрика работает с неполной переданной ей иерархией, главное, чтобы все промежуточные от базового класса до нашего конкретного существовали и у них был определён конструктор.

Если в цепочке иерархии снизу вверх встречается класс, который нельзя сконсутрировать по умолчанию, создастся ближайший родительский класс, который можно создать по умолчанию. Этот случай показывается в последнем тесте

*вместо auto можно было прописать для конкретной иерархии using AbstractFactory, но я считаю, что с auto проще и красивее*
