// ======================================
//	Relógio digital em assembly
// ======================================
//	Dupla:
//		Erick Suzart Souza
//		Valdiney
// ======================================

sfr P0 = 0x80;  //
sfr P1 = 0x90;  //
sfr P2 = 0xA0;  //
sfr P3 = 0xB0;  //

// Portas para o display
sbit DISPLAY_1      = P2^0;
sbit DISPLAY_2      = P2^1;
sbit DISPLAY_3      = P2^2;
sbit DISPLAY_4      = P2^3;
sbit DISPLAY_0      = P2^4;
sbit DISPLAY_5      = P2^5;
sbit DISPLAY_6      = P2^6;
sbit DISPLAY_7      = P2^7;

// Botões para mudar o display
sbit BOTAO_DATA     = P3^0; // Mostra data nos displays
sbit BOTAO_DIA      = P3^1; // Mostra dia da semana: DOM, SEG, TER, QUA, QUI, SEX, SAB.

// Botões de ajustes
sbit a_SEG     = P3^2;  // Zera SegundOo
sbit a_MIN     = P3^3;  // Ajuste Minuto
sbit a_HORA    = P3^4;  // Ajuste Hora
sbit a_DIA     = P3^5;  // Ajuste Dia
sbit a_MES     = P3^6;  // Ajuste Mês
sbit a_ANO     = P3^7;  // Ajuste Ano

// char = 1 byte
// HORAS   DATA
unsigned char COD0 ; //  US		UA
unsigned char COD1 ; //  DS		DA
unsigned char COD2 ; //  .		-
unsigned char COD3 ; //  UM		UM
unsigned char COD4 ; //  DM		DM
unsigned char COD5 ; //  .		-
unsigned char COD6 ; //  UH		UD
unsigned char COD7 ; //  DH		DD

unsigned char SEGUNDOS ;
unsigned char MINUTOS ;
unsigned char HORAS ;
unsigned char DIAS ;
unsigned char MESES ;
unsigned char ANOS ;
unsigned char DIA_SEMANA ;

// Variável de controle
unsigned int count;

unsigned char displayCode [10] =
{
  0xc0,   // 0 código do zero
  0xf9,   // 1
  0xa4,   // 2
  0xb0,   // 3
  0x99,   // 4
  0x92,   // 5
  0x82,   // 6
  0xf8,   // 7
  0x80,   // 8
  0x90    // 9
};

unsigned char Month_Offset [12] =
{
  0x0,    // janeiro
  0x3,    // fevereiro
  0x3,    // março
  0x6,    // abril
  0x1,    // maio
  0x4,    // junho
  0x6,    // julho
  0x2,    // agosto
  0x5,    // setembro
  0x0,    // outubro
  0x3,    // novembro
  0x5     // dezembro
};

// Protótipos de funções
extern void delay(void);
void ClockReset(void);
void DisplayUpdate(void);
void MostrarData(void);
void MostrarHora(void);
void MostrarDia(void);
void VerificarBotoes(void);
void AjustarMinutos(void);
void AjustarHoras(void);
void AjustarDias(void);
void AjustarMes(void);
void AjustarAno(void);
void AtualizarData(void);
void IncrementaMes (unsigned char dia);
void IncrementaAno (void);

void main (void)
{
  ClockReset();
  while(1)
  {
    DisplayUpdate();
    if (a_SEG == 0)
      SEGUNDOS = 0;
    if (a_MIN == 0)
      AjustarMinutos();
    if (a_HORA == 0)
      AjustarHoras();
    if (a_DIA == 0)
      AjustarDias();
    if (a_MES == 0)
      AjustarMes();
    if (a_ANO == 0)
      AjustarAno();
    if (++SEGUNDOS == 60)
    {
      SEGUNDOS = 0;
      if (++MINUTOS == 60)
      {
        MINUTOS = 0;
        if (++HORAS == 24)
        {
          HORAS = 0;
          AtualizarData();
        }
      }
    }
  }
}

/**
 * @brief Reinicia o relógio com valores padrão
 * 
 */
void ClockReset(void)
{
  // Reset:
  DISPLAY_0 = 0;
  DISPLAY_1 = 1;
  DISPLAY_2 = 1;
  DISPLAY_3 = 1;
  DISPLAY_4 = 1;
  DISPLAY_5 = 1;
  DISPLAY_6 = 1;
  DISPLAY_7 = 1;

  SEGUNDOS  = 0;
  MINUTOS   = 0;
  HORAS     = 12;
  DIAS      = 28;
  MESES     = 5;
  ANOS      = 21;
}

/**
 * @brief Atualiza o display por varredura durante um segundo.
 * 
 */
void DisplayUpdate(void)
{
  for (count = 0; count != 480; count++ )
  {
    VerificarBotoes();
    if ( DISPLAY_0 == 0 )
    {
      DISPLAY_0 = 1;
      P0 = COD1;
      DISPLAY_1 = 0;
    }
    else if ( DISPLAY_1 == 0 )
    {
      DISPLAY_1 = 1;
      P0 = COD2;
      DISPLAY_2 = 0;
    }
    else if ( DISPLAY_2 == 0 )
    {
      DISPLAY_2 = 1;
      P0 = COD3;
      DISPLAY_3 = 0;
    }
    else if ( DISPLAY_3 == 0 )
    {
      DISPLAY_3 = 1;
      P0 = COD4;
      DISPLAY_4 = 0;
    }
    else if ( DISPLAY_4 == 0 )
    {
      DISPLAY_4 = 1;
      P0 = COD5;
      DISPLAY_5 = 0;
    }
    else if ( DISPLAY_5 == 0 )
    {
      DISPLAY_5 = 1;
      P0 = COD6;
      DISPLAY_6 = 0;
    }
    else if ( DISPLAY_6 == 0 )
    {
      DISPLAY_6 = 1;
      P0 = COD7;
      DISPLAY_7 = 0;
    }
    else if ( DISPLAY_7 == 0 )
    {
      DISPLAY_7 = 1;
      P0 = COD0;
      DISPLAY_0 = 0;
    }
    delay(); // 2.08 ms
  }
}

/**
 * @brief Verifica qual botão está pressionado e configura o display em função
 * do mesmo
 *
 */
void VerificarBotoes(void)
{
  if(BOTAO_DATA == 0)
    MostrarData();
  else if (BOTAO_DIA == 0)
    MostrarDia();
  else
    MostrarHora();
}

/**
 * @brief Mostra a data no display
 * 
 */
void MostrarData(void)
{
  COD0 = displayCode [ANOS % 10];
  COD1 = displayCode [ANOS / 10];
  COD2 = 0xbF;
  COD3 = displayCode [MESES % 10];
  COD4 = displayCode [MESES / 10];
  COD5 = 0xbF;
  COD6 = displayCode [DIAS % 10];
  COD7 = displayCode [DIAS / 10];
}

/**
 * @brief Mostra a hora no display
 * 
 */
void MostrarHora(void)
{
  COD0 = displayCode [SEGUNDOS % 10];
  COD1 = displayCode [SEGUNDOS / 10];
  COD2 = 0x7F;
  COD3 = displayCode [MINUTOS % 10];
  COD4 = displayCode [MINUTOS / 10];
  COD5 = 0x7F;
  COD6 = displayCode [HORAS % 10];
  COD7 = displayCode [HORAS / 10];
}

/**
 * @brief Mostra o dia da semana no display
 * 
 */
void MostrarDia(void)
{
  DIA_SEMANA = (Month_Offset[MESES - 1] + DIAS + ANOS + ANOS/4 + 6) % 7;

  if (ANOS % 4 == 0) // Ano bissexto
    DIA_SEMANA--;

  switch (DIA_SEMANA)
  {
  case 0: // Domingo
    COD7=0xFF; COD6=0xA1; COD5=0xA3; COD4=0xC8; COD3=0xCF; COD2=0xAB; COD1=0xC2; COD0=0xA3; break;
  case 1: // Segunda
    COD7=0xFF; COD6=0x92; COD5=0x86; COD4=0xC2; COD3=0xc1; COD2=0xAB; COD1=0xA1; COD0=0x88; break;
  case 2: // Terça
    COD7=0xFF; COD6=0xFF; COD5=0xFF; COD4=0x87; COD3=0x86; COD2=0xAF; COD1=0xC6; COD0=0x88; break;
  case 3: // Quarta
    COD7=0xFF; COD6=0xFF; COD5=0x98; COD4=0xC1; COD3=0x88; COD2=0xAF; COD1=0x87; COD0=0x88; break;
  case 4: // Quinta
    COD7=0xFF; COD6=0xFF; COD5=0x98; COD4=0xC1; COD3=0xCF; COD2=0xAB; COD1=0x87; COD0=0x88; break;
  case 5: // Sexta
    COD7=0xFF; COD6=0xFF; COD5=0xFF; COD4=0x92; COD3=0x86; COD2=0x89; COD1=0x87; COD0=0x88; break;
  case 6: // Sábado
    COD7=0xFF; COD6=0xFF; COD5=0x92; COD4=0x88; COD3=0x83; COD2=0x88; COD1=0xA1; COD0=0xA3; break;
  }
}

/**
 * @brief Ajusta os minutos incrementando os mesmos.
 * 
 */
void AjustarMinutos(void)
{
  if (++MINUTOS == 60)
    MINUTOS = 0;
}

/**
 * @brief Ajusta as horas incrementando-as
 * 
 */
void AjustarHoras(void)
{
  if (++HORAS == 24)
    HORAS = 0;
}

/**
 * @brief Ajusta os dias por incremento.
 * 
 */
void AjustarDias(void)
{
  AtualizarData();
}

/**
 * @brief Ajusta o mês por incremento.
 * 
 */
void AjustarMes(void)
{
  if (++MESES > 12)
    MESES = 1;
}

/**
 * @brief Ajusta o ano por incremento
 * 
 */
void AjustarAno(void)
{
  if (++ANOS > 99)
    ANOS = 0;
}

/**
 * @brief Incrementa os dias e atualiza a data em função do mês.
 * 
 */
void AtualizarData(void)
{
  DIAS++;

  switch (MESES)
  {
  case 1: // janeiro
    IncrementaMes(31); break;
  case 2: // fevereiro
    if (ANOS % 4 == 0) // Ano bissexto
      IncrementaMes(29);
    else
      IncrementaMes(28); break;
  case 3: // março
    IncrementaMes(31); break;
  case 4: // abril
    IncrementaMes(30); break;
  case 5: // maio
    IncrementaMes(31); break;
  case 6: // junho
    IncrementaMes(30); break;
  case 7: // julho
    IncrementaMes(31); break;
  case 8: // agosto
    IncrementaMes(31); break;
  case 9: // setembro
    IncrementaMes(30); break;
  case 10: // outubro
    IncrementaMes(31); break;
  case 11: // novembro
    IncrementaMes(30); break;
  case 12: // dezembro
    IncrementaAno(); break;
  }
}

/**
 * @brief Incrementa o mês em função do parâmetro "dia", que define o último dia
 * do mês.
 *
 * @param d Último dia do mês
 */
void IncrementaMes (unsigned char d)
{
  if (DIAS > d)
  {
    DIAS = 1;
    MESES++;
  }
}

/**
 * @brief Incrementa o ano
 * 
 */
void IncrementaAno (void)
{
  IncrementaMes(31);
  MESES = 1;

  if (++ANOS > 99)
    ANOS = 0;
}