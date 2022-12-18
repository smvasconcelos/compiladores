

O projeto 2a consiste em codificar um simulador de um autômato pilha que reconhece a nossa linguagem e que gere a arvore de sintatica codificada no vetor n-ario.

Exemplo:

  /*
    ! p12: X → +
    ! p13: X -> -
    ! p14: X -> *
    ! p15: X -> /
  */
  // Empilha
  if (token == '+' && STACK[TOP] == 'X')
  {
    pop();
    push('+');
    return 1;
  }
  else if (token == '-' && STACK[TOP] == 'X')
  {
    pop();
    push('-');
    return 1;
  }
  else if (token == '*' && STACK[TOP] == 'X')
  {
    pop();
    push('*');
    return 1;
  }
  else if (token == '/' && STACK[TOP] == 'X')
  {
    pop();
    push('/');
    return 1;
  }
  // Consome
  else if (token == '+' && STACK[TOP] == '+')
  {
    pop();
    return 1;
  }
  else if (token == '-' && STACK[TOP] == '-')
  {
    pop();
    return 1;
  }
  else if (token == '*' && STACK[TOP] == '*')
  {
    pop();
    return 1;
  }
  else if (token == '/' && STACK[TOP] == '/')
  {
    pop();
    return 1;
  }


O 2c é um analisador descendente recursivo aka um monte de if aninhado pra cada produção.

ex:

obs: lex é uma função que move pro próximo caracter da string ou arquivo. A função erro é algo generico que mostra qual erro ocorreu, estava lend otal char e esperava tal coisa

  // p6: M → m(){ C; r(E); }
  int M(){
    int err = 0;
    if(token=='m'){ lex();
      if(token=='('){ lex();
        if(token==')'){ lex();
          if(token=='{'){ lex();
            C();
            if(token==';'){ lex();
              if(token=='r'){ lex();
                if(token=='('){ lex();
                  E();
                  if(token==')'){ lex();
                    if(token==';'){ lex();
                      if(token=='}'){ lex();}
                      else { erro(++err, 'M', "}");}
                    }//if-;
                    else { erro(++err, 'M', ";");}
                  }//if-)
                  else { erro(++err, 'M', ")");}
                }//if-(
                else { erro(++err, 'M', "(");}
              }//if-r
              else { erro(++err, 'M', "r");}
            }//if-;
            else { erro(++err, 'M', ";");}
          }//if-{
          else { erro(++err, 'M', "{");} // não veio {
        }//if-)
        else { erro(++err, 'M', ")");} // não veio )
      }//if-(
      else { erro(++err, 'M', "(");} // não veio (
    }//if-m
    else { erro(++err, 'M', "m"); } // não veio m
  }//end-M

O projeto 2c é o 2a com algumas modificações, agora vc tem que mostrar as produções e transições que foram utilizadas em cada processo de empilhar e consumir cada token.

Mostrar se a frase foi aceita ou se deu erro e em qual char deu erro.

Essa parte é confusa, por que esse é o projeto 3a essencialmente, então não entendi muito bem ->

e também Deve ser impressa a árvore de sintaxe abstrata binária,
codificada num vetor de inteiros. Á Árvore de Sintaxe
Abstrata é obtida da Árvore de Analise, removendo os
símbolos sem conteúdo semântico

Projeto 3a ->
Gerar, a partir da árvore de análise (Projeto 2c)
a árvore de sintaxe abstrata correspondente às
estruturas sintáticas da linguagem projetada.

-> Nesse aqui basicamente vc tem que pegar a arvore n-aria gerada loopar ela e identificar os  filhos corretamente e ir montando uma arvore n-aria com ponteiros pra poder fazer as operações de rotação depois e montar a arvore de sintaxe abstrata que é aquela arvore reduzida que quando vc roda um pos ordem vc gera a notação polonesa.


projeto 2d eu não fiz e não planejo fazer, é o único sem ligação entre todos os outros e só ia me atrasar em entregar o resto. Mas ele é pra fazer um simulador da tabela dos alritmos bottom up, aquele que a gnete só fez 2 exercicios em aula da tabela que só era indexar as coisas e dar um shift reduce etc.
