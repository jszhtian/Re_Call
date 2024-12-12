class Person
{
    String name;
    int age;
    void talk()
    {
        System.out.println(name);
        System.out.println(age);
    }
    void setName(String name)
    {
        this.name = name;
    }
    void setAge(int age)
    {
        this.age = age;
    }

}

public class test
{
    public static void main(String[] args)
    {
        Person p = new Person();
        p.setName("Jerry Shen");
        p.setAge(32);
        p.talk();
    }
}