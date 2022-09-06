//
//  main.c
//  ?
//
//  Created by Michael Babušík on 16.10.2021.
//

// Seznam pravidel:

/*
 1 - Heslo obsahuje alespoň 1 velké a 1 malé písmeno.
 2 - Heslo obsajuje znaky z alespoň X skupin. Uvažované skupiny jsou:
   - malá písmena (a-z)
   - velká písmena (A-Z)
   - čísla (0-9)
   - speciální znaky (podporované musí být alespoň nealfanumerické znaky z ASCII tabulky na pozicích 33-126 32-126, tedy včetně mezery)
 3 - Heslo neobsahuje sekvenci stejných znaků délky alespoň X.
 4 - Heslo neobsahuje dva stejné podřetězce délky alespoň X.
 */
#include <stdio.h>
#include <stdlib.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//funkce jsou popsané pod mainem
int LEVEL1(char password[]);
int LEVEL2(char password[],int param);
int LEVEL3(char password[], int param);
int LEVEL4(char password[], int param);
int arg(const char retezec[]);
int statistika(char x[],const char d [],int argc);
int delka_hesla(char password[]);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

int main(int argc,char const *argv[])
{
    //vypisování chyb do erroru
   if (argc < 3)
   {
       fprintf(stderr,"Zadali jste malo argumentu\n");
       return -1;
   }
   if (argc > 4)
   {
       fprintf(stderr,"Zadali jste moc argumentu\n");
       return -1;
   }
    if ((arg(argv[1])==1)||(arg(argv[2])==1))
    {
        fprintf(stderr,"argument pro LEVEL a PARAM musi byt CISLO!\n");
        return -1;
    }
    //atoi = converter ze stringu na integer
    int level = atoi (argv[1]);
    int param = atoi (argv[2]);
    
    if (level<1||level>4)
    {
        fprintf(stderr,"LEVEL musi byt v intervalu 1-4\n");
        return -1;
    }
    if ((param<1)||(param>100))
    {
        fprintf(stderr,"PARAM musi byt minimalne 1\n");
        return -1;
    }
    
    char password [102];
    char allChars [102];
    allChars[0] = '\0';
    float pocet_hesel = 0;
    float pocet_znaku = 0;
    int MIN = 100;
    float AVG = 0;
    int ruzne_znaky = 0;
    
    //vezme hesla ze standardniho (fgets) inputu a vloží na standardni output (fputs)
    while (fgets (password, 102, stdin)!=NULL)
    {
        //počítání různych znaků
        for(int i = 0;password[i] != '\n';i++)
          {
              int nalezeno = 0;
              int a = 0;
              while (allChars[a] != '\0' && nalezeno != 1)
              {
                  if (password[i] == allChars[a]) nalezeno = 1;
                  else a++;
              }

              if (nalezeno != 1)
              {
                  ruzne_znaky++;
                  allChars[a] = password[i];
                  allChars[a + 1] = '\0';
              }
          }
        
        
        pocet_znaku+=delka_hesla(password);
        pocet_hesel++;
        AVG = (pocet_znaku/pocet_hesel);
        if(MIN >= delka_hesla(password))
        {
            MIN = (delka_hesla(password));
        }
        
        //volání funkcií pravidel podle argumentu level
        //každá funkce levelu vrací 1 pro heslo, ktere prošlo a když heslo neprošlo, vrací 0
          if (level==1)
          {
                if (LEVEL1(password)==0)
                {
                    continue;
                }
          }
          if (level==2)
          {
                if ((LEVEL1(password) + LEVEL2(password,param)) < 2)
                {
                    continue;
                }
          }
          if (level==3)
          {
              if ((LEVEL1(password) + LEVEL2(password,param) + LEVEL3(password,param)) < 3)
              {
                  continue;
              }
          }
          if (level==4)
          {
              if ((LEVEL1(password) + LEVEL2(password,param) + LEVEL3(password,param) + LEVEL4(password,param)) < 4)
              {
                  continue;
              }
          }
        fputs(password,stdout);
    }
    //Jestli tam je napsane --stats, vypíše statistiku a hesla odpovídající argumentum levelu a parametru
    //Jestli je --stats napsané špatně, nevypíše statistiku, ale pořád vypíše hesla odpovídající argumentum levelu a parametru + napíše error, že jestli chtěli vidět statistiku, musí to napsat správně
    //statistika
    if (statistika("--stats", argv[3],argc)==1)
    {
        printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %0.1f\n", ruzne_znaky, MIN, AVG);
    }
    return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//zjišťuje výskyt velkých a malých písmen v hesle tím, že je spočítá a porovná s 0
int LEVEL1(char password[])
{
    //m počet malých písmen
    //v počet velkých písmen
    int i = 0, v = 0, m = 0;
  
    while (password[i]!='\0')
    {
        if (password[i]>='a' && password[i]<='z')
        {
            m++;
        }
        else if (password[i]>='A' && password[i]<='Z')
        {
            v++;
        }
        i++;
    }
    
    //jestli počet velkých i malých písmen bude větší než 0, heslo prošlo.
    if (v > 0 && m > 0)
    {
        return 1;
    }
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
/*2 - Heslo obsajuje znaky z alespoň X skupin. Uvažované skupiny jsou:
  - malá písmena (a-z)
  - velká písmena (A-Z)
  - čísla (0-9)
  - speciální znaky (podporované musí být alespoň nealfanumerické znaky z ASCII tabulky na pozicích 32-126, tedy včetně mezery)*/
//zjistištuji obdobně jako v prvni levelu výskyt m, v, c, s a porovnám s nulou, jestliže najdu výskyt, převedu proměnou na 1, na konci sečtu a výsledek porovnám s parametrem
int LEVEL2(char password[],int param)
{
    //m počet malých písmen
    //v počet velkých písmen
    //c počet čísel
    //s počet speciálních znaků 32-126
    int i = 0, m = 0, v = 0, c = 0, s = 0;
    //kdyby někdo zadal větší param než je počet pravidel, bude brát přesně ty 4 pravidla
    if (param>=4)
    {
        param = 4;
    }
    while (password[i]!='\0')
    {
        if(password[i]>='a'&&password[i]<='z')
        {
            m++;
            if(m>0)
            {
                m = 1;
            }
        }
        else if(password[i]>='A'&&password[i]<='Z')
        {
            v++;
            if(v>0)
            {
                v = 1;
            }
        }
        else if(password[i]>='0'&&password[i]<='9')
        {
            c++;
            if(c>0)
            {
                c = 1;
            }
        }
        else if((password[i]>=32 && password[i]<=47)||(password[i]>=58 && password[i]<=64)||(password[i]>=91 && password[i]<=96)||(password[i]>=123 && password[i]<=126))
        {
            s++;
            if(s>0)
            {
                s = 1;
            }
        }
        i++;
    }
    if (m+v+c+s>=param)
    {
        return 1;
    }
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//3 - Heslo neobsahuje sekvenci stejných znaků délky alespoň X.
/*
 Prochází heslo a porovnává ho s předchozím znakem, jestli se rovná ke k se přičte jednička
 jestli je k větší než maxSequention nebo jemu rovno, přepíše se do něho.
 Jestli se dva po sobě jdoucí znaky nerovnají, k je 1 jako sekvence jednoho znaku
 jestli největší sekvence stejných znaků v hesle je menší než parametr, heslo prošlo a funkce vrací 1
 */
int LEVEL3(char password[], int param)
{
    if(param==1)
    {
        return 0;
    }
    int i = 1;
    int k = 1;
    int maxSequention = 0;
    while(password[i]!='\0')
    {
        if(password[i]==password[i-1])
        {
            k++;
            if(k>=maxSequention)
            {
                maxSequention=k;
            }
        }
        else
        {
            k = 1;
        }
        
        i++;
    }
    if(maxSequention<param)
    {
        return 1;
    }
    return 0;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// 4 - Heslo neobsahuje dva stejné podřetězce délky alespoň X.
// pocita stejné podřetězce ve stringu
//check a control: substringy
//pocitadlo: delka stejnych podretezcu
//když má heslo nejdelší délku podřetězce délku param. vrací nulu, když heslo v poho splnujě, vrací jedničku
int LEVEL4(char password[],int param)
{
    char control[100];
    char check[100];
    for (int i=0; password[i+param-1]!='\0'; i++)
    {
        for (int x=0;x<param; x++)
        {
            control[x]=password[i+x];
        }
        control[param]='\0';
        for (int h=i+1; password[h+param-1]!='\0'; h++)
        {
            for (int k=0;k<param; k++)
            {
                check[k]=password[h+k];
            }
            check[param]='\0';
            
            int pocitadlo = 0;
            
            for (int z=0; z<param; z++)
            {
                if(control[z]==check[z])
                {
                    pocitadlo++;
                }
                if(pocitadlo==param)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//čekuje, jestli je v argumentu pismeno
//Jestli v argumentu je písmeno, vrací se mi jednicka a argument je neplatný
int arg(const char retezec[])
{
    int p = 0;
    for (int i=0; retezec[i]!='\0'; i++)
    {
        if(retezec[i]<'0'||retezec[i]>'9')
        {
            p++;
        }
        if (p > 0)
        {
            return 1;
        }
    }
    return 0;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//zjišťuje, jestli je tam napsané --stats a jestli ano, returne 1
            //--stats  argv[3]=--haha
int statistika(char x[],const char d [],int argc)
{
    
    if (argc==4)
    {
        int l=0;
        for (;x[l]!='\0' || d[l]!='\0'; l++)
        {
            if (x[l]!=d[l])
            {
                fprintf(stderr,"Jestli jste chteli videt statistiku, musite napsat --stats\n");
                return -1;
            }
        }
        if(x[l]!=d[l])
        {
            return 1;
        }
        return 1;
    }
    return 0;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//zjištuje delku stringu
int delka_hesla(char password[])
{
    int i = 0;
    while (password[i]!='\n')
    {
        i++;
    }
    return i;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
