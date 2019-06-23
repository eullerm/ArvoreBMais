// Executa busca em Arquivos utilizando Arvore B+
// Assumir que ponteiro para prpximo noh eh igual a -1 quando nao houver proximo noh
// cod: chave da pizza que esta sendo buscada
// nome_arquivo_metadados: nome do arquivo que contem os metadados
// nome_arquivo_indice: nome do arquivo de indice (que contem os nohs internos da arvore B+)
// nome_arquivo_dados: nome do arquivo de dados (que contem as folhas da arvore B+)
// d: ordem da arvore
// Retorna ponteiro para nó em que a chave está ou deveria estar
int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

// Executa insercao em Arquivos Indexados por Arvore B+
// cod: chave da pizza que esta sendo inserida
// nome: nome da pizza a ser inserida
// categoria: categoria da pizza a ser inserida
// preco: preco da pizza a ser inserida
// nome_arquivo_metadados: nome do arquivo de metadados
// nome_arquivo_indice: nome do arquivo de indice (que contem os nohs internos da arvore B+)
// nome_arquivo_dados: nome do arquivo de dados (que contem as folhas da arvore B+)
// d: ordem da arvore
// Retorna o ponteiro para a folha onde o registro foi inserido
int insere(int cod, char *nome, char *categoria, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

// Executa exclusao em Arquivos Indexados por Arvores B+
// cod: chave da pizza a ser excluida
// nome_arquivo_metadados: nome do arquivo que contem os metadados
// nome_arquivo_indice: nome do arquivo de indice (que contem os nohs internos da arvore B+)
// nome_arquivo_dados: nome do arquivo de dados (que contem as folhas da arvore B+)
// d: ordem da arvore
// Retorna o ponteiro para a folha onde o registro foi excluido
int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d);

// Le os dados do arquivo nome_arquivo_entrada e cria arvore B+ de ordem d
// grava a arvore B+ nos arquivos a seguir:
// nome_arquivo_metadados: nome do arquivo para guardar os metadados
// nome_arquivo_indice: nome do arquivo para guardar o indice (nohs internos da arvore B+)
// nome_arquivo_dados: nome do arquivo para guardar os dados (nohs folhas da arvore B+)
void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados);
