#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 100
#define AG "AggiungiGrafo\n"
#define TOPK "TopK\n"
#define MAX_UINT 4294967295
#define MAX_UINT_CHARS 12
#define ZERO 48

typedef struct _edge {
	u_int32_t key;
	u_int32_t dist;
	u_int32_t queueIndex;
} edge_t;

typedef struct _rank {
	u_int32_t index;
	u_int64_t sum;
} rank_t;

u_int32_t stou(char**);
void strConcat(char**, char*);
u_int32_t utos(char*, u_int32_t);

void replaceMax(rank_t*, u_int32_t, u_int64_t, u_int32_t);
void deleteMax(rank_t*, u_int32_t);
void maxHeapify(rank_t*, u_int32_t, u_int32_t);
void buildMaxHeap(rank_t*, u_int32_t);
void insertPriority(rank_t*, u_int32_t, u_int64_t, u_int32_t);
void printMaxHeap(rank_t*, u_int32_t);

u_int64_t addGraph(u_int64_t);
edge_t* deleteMin(edge_t**, u_int32_t*);
void minHeapify(edge_t**, u_int32_t, u_int32_t);
void buildMinHeap(edge_t**, u_int32_t);

int main(int argc, char* argv[]) {
	u_int32_t k, counter;
	u_int64_t d, currSum;
	char buffer[(MAX_UINT_CHARS + 1) * 2], cmd[LEN+1];
	char *end;

	if(fgets(buffer, (MAX_UINT_CHARS + 1) * 2, stdin)) {
		d = strtol(buffer, &end, 10);
		end++;
		k = strtol(end, &end, 10);
	} else {
		return 1;
	}

	// inizializzo array che virtualizza max heap per gestire la classifica
	rank_t ranking[k];

	// inizializzo counter a 0 per tenere traccia dell'indice di ogni grafo
	counter = 0;

	while(fgets(cmd, LEN, stdin) != NULL) {
		if(strcmp(cmd, AG) == 0) {
			currSum = addGraph(d);

			if(counter == k)
				buildMaxHeap(ranking, k-1);

			if(counter < k) {
				ranking[counter].index = counter;
				ranking[counter].sum = currSum;
			} else if(currSum < ranking[0].sum) {
				deleteMax(ranking, k);
				insertPriority(ranking, counter, currSum, k-1);
			}

			counter++;
		} else if(strcmp(cmd, TOPK) == 0) {
			printMaxHeap(ranking, (counter < k) ? counter : k);
		}
	}

	return 0;
}

void deleteMax(rank_t ranking[], u_int32_t heapsize) {
	if(heapsize == 0) {
		return;
	}

	ranking[0] = ranking[heapsize-1];
	heapsize--;
	maxHeapify(ranking, 0, heapsize);
}

void replaceMax(rank_t ranking[], u_int32_t index, u_int64_t sum, u_int32_t heapsize) {
	if(heapsize == 0) return;

	ranking[0].index = index;
	ranking[0].sum = sum;
	maxHeapify(ranking, 0, heapsize);
}

void maxHeapify(rank_t ranking[], u_int32_t n, u_int32_t heapsize) {
	u_int32_t l, r, posmax;
	rank_t temp;

	l = 2*n + 1;
	r = 2*n + 2;

	if(l < heapsize && ranking[l].sum > ranking[n].sum)
		posmax = l;
	else
		posmax = n;
	if(r < heapsize && ranking[r].sum > ranking[posmax].sum)
		posmax = r;

	if(posmax != n) {
		temp = ranking[posmax];
		ranking[posmax] = ranking[n];
		ranking[n] = temp;

		maxHeapify(ranking, posmax, heapsize);
	}
}

void buildMaxHeap(rank_t ranking[], u_int32_t heapsize) {
	if(heapsize == 0) return;

	int32_t i;

	for(i = (heapsize-1) / 2; i >= 0; i--)
		maxHeapify(ranking, i, heapsize);
}

void insertPriority(rank_t ranking[], u_int32_t index, u_int64_t sum, u_int32_t heapsize) {
	u_int32_t i, parent;
	rank_t temp;

	ranking[heapsize].index = index;
	ranking[heapsize].sum = sum;

	i = heapsize;
	parent = (i-1) / 2;
	while(i > 0 && ranking[parent].sum < ranking[i].sum) {
		temp = ranking[i];
		ranking[i] = ranking[parent];
		ranking[parent] = temp;

		i = parent;
		parent = (i-1) / 2;
	}
}

void strConcat(char **buffer, char *new) {
	while(**buffer)
		*buffer = *buffer + 1;

	while(*new) {
		**buffer = *new;
		*buffer = *buffer + 1;
		new = new + 1;
	}

	**buffer = '\0';
}

u_int32_t utos(char str[], u_int32_t n) {
	if(n == 0) {
		*str = '0';
		str = str + 1;
		*str = '\0';
		return 1;
	}

	char temp;
	u_int32_t i, size = 0;

	while(n > 0) {
		str[size] = (n % 10) + 48;
		n /= 10;
		size++;
	}

	for(i = 0; i < size / 2; i++) {
		temp = str[i];
		str[i] = str[size - i - 1];
		str[size - i - 1] = temp;
	}
	str[size] = '\0';

	return size;
}

void printMaxHeap(rank_t ranking[], u_int32_t heapsize) {
	u_int32_t i, written = 0;
	char conv[MAX_UINT_CHARS] = "";
	char b[8192] = "";
	char *buffer = b;

	for(i = 0; i < heapsize; i++) {
		if(written > 8150) {
			printf("%s", b);
			buffer = b;
			buffer[0] = '\0';
		}

		if(i) {
			strConcat(&buffer, " ");
			written++;
		}
		written += utos(conv, ranking[i].index);
		strConcat(&buffer, conv);
	}

	printf("%s\n", b);
}

u_int32_t stou(char **start) {
	u_int32_t parsedValue = 0;
	u_int32_t	currCh = (u_int32_t) **start;

	while(currCh >= ZERO) {
		parsedValue *= 10;
		parsedValue += (currCh - ZERO);

		*start = *start + 1;
		currCh = (u_int32_t) **start;
	}

	*start = *start + 1;
	return parsedValue;
}

u_int64_t addGraph(u_int64_t d) {
	u_int32_t i, j, heapsize, currKey, adjDist, newDist, queueIdx;
	u_int32_t mat[d][d];
	u_int64_t sum;
	edge_t *u;
	edge_t edges[d], *queue[d];

	// inizializzo la somma dei cammini minimi a 0
	sum = 0;

	// leggo la matrice di adiacenza dall'input
	char buffer[MAX_UINT_CHARS * d], *startPtr;
	for(i = 0; i < d; i++) {
		if(fgets(buffer, MAX_UINT_CHARS * d, stdin) != NULL) {
			startPtr = &buffer[0];
			for(j = 0; j < d; j++) {
				mat[i][j] = stou(&startPtr);
			}
		}
	}

	// inizializzo i nodi con distanza 0 (nodo sorgente) oppure MAX_UINT
	for(i = 0; i < d; i++) {
		edges[i].key = i;
		edges[i].dist = MAX_UINT;
		edges[i].queueIndex = i;
	}
	edges[0].dist = 0;

	// inizializzo min-heap queue con puntatori a tutti i nodi
	for(i = 0; i < d; i++) {
		queue[i] = &edges[i];
	}

	// visito tutti gli archi, cercando i cammini minimi
	heapsize = d;
	while(heapsize > 0) {
		u = deleteMin(queue, &heapsize);

		// se u è raggiungibile sommo u->dist alla somma dei cammini minimi;
		// altrimenti non sommo più nulla, poiché significa che nessun altro nodo è raggiungibile
		if(u->dist == MAX_UINT)
			heapsize = 0;
		else {
			// aggiungo la distanza del nodo più vicino alla somma dei cammini minimi
			sum += u->dist;

			// scorro la riga della matrice di adiacenza corrispondente a u
			currKey = u->key;
			for(i = 0; i < d; i++) {
				adjDist = mat[currKey][i];
				queueIdx = edges[i].queueIndex;

				// considero solo i nodi adiacenti, eccetto l'autoanello
				// verifico se la distanza dal nodo sorgente diminuisce passando per u
				if(i != currKey && queueIdx < heapsize && adjDist > 0) {
					newDist = u->dist + adjDist;
					if(edges[i].dist > newDist) {
						edges[i].dist = newDist;
					}
				}
			}

			buildMinHeap(queue, heapsize);
		}
	}

	return sum;
}

edge_t* deleteMin(edge_t *queue[], u_int32_t *heapsize) {
	if(*heapsize == 0)
		return queue[0];

	edge_t *min;

	min = queue[0];
	queue[0] = queue[*heapsize - 1];
	queue[*heapsize - 1] = min;

	queue[0]->queueIndex = 0;
	queue[*heapsize - 1]->queueIndex = *heapsize - 1;

	*heapsize = *heapsize - 1;

	minHeapify(queue, 0, *heapsize);

	return min;
}

void minHeapify(edge_t *queue[], u_int32_t n, u_int32_t heapsize) {
	u_int32_t l, r, posmin;
	edge_t *temp;

	l = 2*n + 1;
	r = 2*n + 2;

	if(l < heapsize && queue[l]->dist < queue[n]->dist)
		posmin = l;
	else
		posmin = n;
	if(r < heapsize && queue[r]->dist < queue[posmin]->dist)
		posmin = r;

	if(posmin != n) {
		temp = queue[posmin];
		queue[posmin] = queue[n];
		queue[n] = temp;

		queue[n]->queueIndex = n;
		queue[posmin]->queueIndex = posmin;

		minHeapify(queue, posmin, heapsize);
	}
}

void buildMinHeap(edge_t *queue[], u_int32_t heapsize) {
	if(heapsize == 0) return;

	int32_t i;

	for(i = (heapsize-1) / 2; i >= 0; i--)
		minHeapify(queue, i, heapsize);
}