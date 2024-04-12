#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// Fill out this structure
struct HashType {
  struct RecordType *r;
  struct HashType *next;
};

// Compute the hash function
int hash(int x) { return x % 1000 % 31; }

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
    // Implement parse data block
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
  int i;

  for (i = 0; i < hashSz; ++i) {
    // if index is occupied with any records, print all
    if (pHashArray[i].r != NULL) {
      // Prints the record
      int id = pHashArray[i].r->id;
      char *name;
      strcpy(name, &pHashArray[i].r->name);
      int order = pHashArray[i].r->order;
      printf("index x -> %d, %s, %d, %d\n", id, name, order, hash(id));

      // Prints all the records in the bucket
      if (pHashArray[i].next != NULL) {
        // Creates the traversal node
        struct HashType *t = &pHashArray[i];

        // Goes until an empty bucket is found
        while (t->next != NULL) {
          t = t->next;

          int id = t->r->id;
          char *name;
          strcpy(name, &t->r->name);
          int order = t->r->order;
          printf("index x -> %d, %s, %d, %d\n", id, name, order, hash(id));
        }
      }
    }
  }
}

int main(void) {
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);
  // Your hash implementation
  // Creates our hash table
  struct HashType *hashArray =
      (struct HashType *)malloc(sizeof(struct HashType) * recordSz);

  // Loads the records into the hash table
  for (int i = 0; i < recordSz; i++) {
    int h = hash(pRecords[i].id);

    // If records r is empty, fill it
    if (hashArray[h].r == NULL) {
      hashArray[h].r = &pRecords[i];
    }

    // If empty, store it in a bucket
    else {
      // Creates the traversal node
      struct HashType *t = &hashArray[h];

      // Goes until an empty bucket is found
      while (t->next != NULL) {
        t = t->next;
      }

      // Fills in the bucket
      t->next = (struct HashType *)malloc(sizeof(struct HashType));
      t->next->r = &pRecords[i];
      t->next->next = NULL;
    }
  }

  // Prints the hash
  displayRecordsInHash(hashArray, recordSz);
}