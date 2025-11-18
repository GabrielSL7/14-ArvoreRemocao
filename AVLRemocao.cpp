#include <iostream>
#include <cstdlib> // Para system("cls") e malloc/free

using namespace std;

/* -----------------------------------------------------------
   Estrutura do nó da Árvore AVL
----------------------------------------------------------- */
struct NO {
    int valor;
    NO* esq;
    NO* dir;
    int altura;       // usado para o balanceamento
};

/* Raiz da árvore */
NO* raiz = NULL;

/* ----------------- Protótipos ----------------------------- */
// Interface principal
void menu();
void inicializar();
void inserir();
void exibir();
void exibirQuantidade();
void buscar();
void remover();

// Funções auxiliares da árvore
NO* insereArvore(NO* no, int valor);
NO* criaNO(int valor);
int elementosArvore(NO* no);
void exibirElementosArvore(NO* no, int espaco, bool direita);
void buscarElementoArvore(NO* no, int valor);
NO* removerArvore(NO* no, int valor);
NO* menorValorNo(NO* no); // Novo protótipo para encontrar o sucessor

// Funções auxiliares de balanceamento
int alturaNo(NO* no);
int fatorBalanceamento(NO* no);
NO* girarDireita(NO* no);
NO* girarEsquerda(NO* no);
int maior(int a, int b);
NO* balancearNo(NO* no);

/* ================= IMPLEMENTAÇÃO ========================= */
// ---------- Funções de interface ---------
int main() {
    menu();
}

void menu() {
    int op = 0;
    while (op != 7) {
        system("cls"); // Limpa console (Windows)
        cout << "========== Menu Arvore AVL ==========\n\n";
        cout << "1 - Inicializar Arvore\n";
        cout << "2 - Exibir quantidade de elementos\n";
        cout << "3 - Inserir elemento\n";
        cout << "4 - Remover elemento\n";
        cout << "5 - Exibir arvore\n";
        cout << "6 - Buscar elemento\n";
        cout << "7 - Sair\n\n";
        cout << "Opcao: ";
        cin >> op;

        switch (op) {
        case 1:
            inicializar();
            break;
        case 2:
            exibirQuantidade();
            break;
        case 3:
            inserir();
            break;
        case 4:
            remover();
            break;
        case 5:
            exibir();
            break;
        case 6:
            buscar();
            break;
        }

        cout << endl;
        if (op != 7) system("pause"); // Aguarda tecla (Windows)
    }
}

void inicializar() {
    raiz = NULL;
    cout << "Arvore inicializada!\n";
}

void inserir() {
    int v;
    cout << "Digite o elemento: ";
    cin >> v;
    raiz = insereArvore(raiz, v);
}

void remover() {
    int v;
    cout << "Digite o elemento a ser removido: ";
    cin >> v;

    // --- Lógica de verificação para melhor feedback ---
    // Buscar o elemento para saber se ele existe antes de chamar a remoção recursiva
    NO* temp = raiz;
    bool encontrado = false;
    while (temp != NULL) {
        if (temp->valor == v) {
            encontrado = true;
            break;
        }
        temp = (v < temp->valor) ? temp->esq : temp->dir;
    }

    if (encontrado) {
        raiz = removerArvore(raiz, v);
        cout << "Elemento " << v << " removido e arvore rebalanceada.\n";
    }
    else {
        cout << "Elemento NAO encontrado na arvore.\n";
    }
}


void exibirQuantidade() {
    cout << "Quantidade de elementos: "
        << elementosArvore(raiz) << "\n";
}

void exibir() {
    if (raiz == NULL) {
        cout << "Arvore vazia!\n";
        return;
    }
    cout << "\n===== Estrutura da Arvore (raiz no topo) =====\n\n";
    exibirElementosArvore(raiz, 0, false);
}

void buscar() {
    int v;
    cout << "Digite o elemento: ";
    cin >> v;
    buscarElementoArvore(raiz, v);
}

// ---------- Criação e Balanceamento ----------

NO* criaNO(int valor) {
    NO* novo = (NO*)malloc(sizeof(NO));
    if (novo == NULL) return NULL;      // Falha de alocação

    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;
    return novo;
}

int alturaNo(NO* no) {
    if (no == NULL) return -1;
    return no->altura;
}

int maior(int a, int b) {
    if (a > b) return a;
    return b;
}

int fatorBalanceamento(NO* no) {
    if (no == NULL) return 0;
    return alturaNo(no->esq) - alturaNo(no->dir);
}

NO* girarDireita(NO* y) {
    NO* x = y->esq;
    NO* T2 = x->dir;

    // Realiza a rotação  
    x->dir = y;
    y->esq = T2;

    // Atualiza as alturas  
    y->altura = maior(alturaNo(y->esq), alturaNo(y->dir)) + 1;
    x->altura = maior(alturaNo(x->esq), alturaNo(x->dir)) + 1;

    return x; // Retorna a nova raiz da subárvore
}

NO* girarEsquerda(NO* x)
{
    NO* y = x->dir;
    NO* T2 = y->esq;

    // Transfere T2 e atualiza ponteiros
    y->esq = x;
    x->dir = T2;

    // Atualiza alturas
    x->altura = maior(alturaNo(x->esq), alturaNo(x->dir)) + 1;
    y->altura = maior(alturaNo(y->esq), alturaNo(y->dir)) + 1;

    return y; // Retorna a nova raiz da subárvore
}

// Função auxiliar para balancear um nó AVL
NO* balancearNo(NO* no) {
    if (no == NULL) return no;
    int fb = fatorBalanceamento(no);

    // Caso Esquerda-Esquerda (FB > 1 e FB(esq) >= 0)
    if (fb > 1 && fatorBalanceamento(no->esq) >= 0)
        return girarDireita(no);

    // Caso Esquerda-Direita (FB > 1 e FB(esq) < 0)
    if (fb > 1 && fatorBalanceamento(no->esq) < 0) {
        no->esq = girarEsquerda(no->esq);
        return girarDireita(no);
    }

    // Caso Direita-Direita (FB < -1 e FB(dir) <= 0)
    if (fb < -1 && fatorBalanceamento(no->dir) <= 0)
        return girarEsquerda(no);

    // Caso Direita-Esquerda (FB < -1 e FB(dir) > 0)
    if (fb < -1 && fatorBalanceamento(no->dir) > 0) {
        no->dir = girarDireita(no->dir);
        return girarEsquerda(no);
    }
    return no;
}

NO* insereArvore(NO* no, int valor) {
        /* Inserção binária normal */
    if (no == NULL) {
        return criaNO(valor);
    }
    if (valor < no->valor) {
        no->esq = insereArvore(no->esq, valor);
    }
    else if (valor > no->valor) {
        no->dir = insereArvore(no->dir, valor);
    }
    else {
                /* Valor já existe – não insere duplicado */
        return no;
    }

        /* Atualiza altura e balanceia (Retrocesso) */
    no->altura = maior(alturaNo(no->esq), alturaNo(no->dir)) + 1;
    return balancearNo(no);
}

/* -----------------------------------------------------------
    Funções de Remoção
----------------------------------------------------------- */

// Função auxiliar para encontrar o menor valor (Sucessor)
NO* menorValorNo(NO* no) {
    NO* atual = no;
    // Percorre sempre para a esquerda para encontrar o menor valor
    while (atual != NULL && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Função principal de remoção em AVL
NO* removerArvore(NO* no, int valor) {
    // Passo 1: Busca recursiva (Caso Base)
    if (no == NULL) {
        return no;
    }

    // Passo 2: Navega pela árvore
    if (valor < no->valor) {
        no->esq = removerArvore(no->esq, valor);
    }
    else if (valor > no->valor) {
        no->dir = removerArvore(no->dir, valor);
    }
    else {
        // Passo 3: Nó encontrado! Aplicar os casos de remoção.

        // ========== CASO 1 e CASO 2: Nó com zero ou um filho ==========
        if (no->esq == NULL || no->dir == NULL) {

            // 'temp' será o único filho (se houver) ou NULL (se for folha)
            NO* temp = no->esq ? no->esq : no->dir;

            // Subcaso 1: Nó folha (sem filhos)
            if (temp == NULL) {
                temp = no;
                no = NULL;
            }
            // Subcaso 2: Nó com um filho
            else {
                // Copia o conteúdo do filho para o nó atual, o filho sobe de posição.
                *no = *temp;
            }

            free(temp);

        }
        else {
            // ========== CASO 3: Nó com dois filhos ==========

            // Passo 3.1: Encontre o Sucessor 
            NO* sucessor = menorValorNo(no->dir);

            // Passo 3.2: Copie o valor do sucessor para o nó atual
            no->valor = sucessor->valor;

            // Passo 3.3: Remova o sucessor de sua posição original (recursão aninhada)
            no->dir = removerArvore(no->dir, sucessor->valor);
        }
    }

    // Se o nó foi removido (no == NULL), retorna NULL
    if (no == NULL) return no;

    // Passo 4: Atualiza altura do nó (Fase de Retrocesso)
    no->altura = maior(alturaNo(no->esq), alturaNo(no->dir)) + 1;

    // Passo 5: Balanceia o nó se necessário (propriedade AVL)
    return balancearNo(no);
}


// ---------- Utilidades ----------
int elementosArvore(NO* no) {
    if (no == NULL) return 0;
    return 1 + elementosArvore(no->esq) + elementosArvore(no->dir);
}

void exibirElementosArvore(NO* no, int espaco, bool direita) {
        /* Impressão recursiva “deitada”:
           └── valor
              ├── ...
              └── ...
        */
    if (no == NULL) return;

    const int DIST = 6; // deslocamento entre níveis
    espaco += DIST;

    // Exibe primeiro a subárvore direita (fica “em cima” no console)
    exibirElementosArvore(no->dir, espaco, true);

    // Impressão do nó atual
    cout << endl;
    for (int i = DIST; i < espaco; i++) cout << " ";

    if (direita)
        cout << "/-- ";
    else
        cout << "\\-- ";

    cout << no->valor << " (A:" << no->altura << " | FB:" << fatorBalanceamento(no) << ")" << endl;

    // Exibe subárvore esquerda
    exibirElementosArvore(no->esq, espaco, false);
}

void buscarElementoArvore(NO* no, int valor) {
    if (no == NULL) {
        cout << "Elemento NAO encontrado.\n";
        return;
    }

    if (valor == no->valor) {
        cout << "Elemento encontrado!\n";
        return;
    }

    if (valor < no->valor)
        buscarElementoArvore(no->esq, valor);
    else
        buscarElementoArvore(no->dir, valor);
}