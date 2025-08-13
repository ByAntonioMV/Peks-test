#include <pbc.h>
#include <stdio.h>
#include <string.h>

// Estructura para las claves públicas
typedef struct {
    element_t g;         // Generador del grupo G1
    element_t h;         // Generador del grupo G1
    element_t server_pk; // Clave pública del servidor (g^alpha)
    element_t user_pk;   // Clave pública del usuario (h^beta)
} pks_keys_t;

// Estructura para las claves privadas
typedef struct {
    element_t server_sk; // Clave privada del servidor (alpha)
    element_t user_sk;   // Clave privada del usuario (beta)
} sks_keys_t;


// 1. Inicialización y Generación de Claves (KeyGen)
void keygen(pairing_t pairing, pks_keys_t *pks, sks_keys_t *sks) {
    // Inicializar los elementos de las claves
    element_init_G1(pks->g, pairing);
    element_init_G1(pks->h, pairing);
    element_init_G1(pks->server_pk, pairing);
    element_init_G1(pks->user_pk, pairing);
    element_init_Zr(sks->server_sk, pairing);
    element_init_Zr(sks->user_sk, pairing);

    // Generar elementos aleatorios
    element_random(pks->g);
    element_random(pks->h);
    element_random(sks->server_sk); // alpha
    element_random(sks->user_sk);   // beta

    // Calcular las claves públicas
    // server_pk = g^alpha
    element_pow_zn(pks->server_pk, pks->g, sks->server_sk);
    // user_pk = h^beta
    element_pow_zn(pks->user_pk, pks->h, sks->user_sk);

    printf("claves generadas exitosamente.\n");
}

// 2. Generación del Criptograma PEKS
void peks(element_t *peks_A, element_t *peks_B, char *keyword, pks_keys_t *pks, pairing_t pairing) {
    element_t H1_kw, r;
    
    element_init_G1(H1_kw, pairing);
    element_init_Zr(r, pairing);
    element_init_GT(*peks_B, pairing);

    // H1: Hashea la palabra clave a un elemento del grupo G1
    element_from_hash(H1_kw, keyword, strlen(keyword));
    
    // Generar un número aleatorio 'r'
    element_random(r);

    // peks_A = g^r
    element_pow_zn(*peks_A, pks->g, r);

    // t = e(server_pk, H1_kw) = e(g^alpha, H1(kw))
    element_t t;
    element_init_GT(t, pairing);
    pairing_apply(t, pks->server_pk, H1_kw, pairing); // t = e(g^alpha, H1(kw))

    // peks_B = t^r = e(g^alpha, H1(kw))^r
    element_pow_zn(*peks_B, t, r);

    printf("PEKS generado para la palabra clave: '%s'\n", keyword);
    
    element_clear(H1_kw);
    element_clear(r);
    element_clear(t);
}

// 3. Generación de la Trampilla (Trapdoor)
void trapdoor(element_t *trapdoor_T, char *keyword, pks_keys_t *pks, sks_keys_t *sks, pairing_t pairing) {
    element_t H1_kw;
    element_init_G1(H1_kw, pairing);
    
    // H1: Hashea la palabra clave a un elemento del grupo G1
    element_from_hash(H1_kw, keyword, strlen(keyword));

    // trapdoor_T = H1(kw)^alpha
    element_pow_zn(*trapdoor_T, H1_kw, sks->server_sk);
    
    printf("Trapdoor generado para la palabra clave: '%s'\n", keyword);
    element_clear(H1_kw);
}

// 4. Prueba (Test)
int test(element_t *peks_A, element_t *peks_B, element_t *trapdoor_T, pairing_t pairing) {
    element_t test_val_1, test_val_2;
    element_init_GT(test_val_1, pairing);
    element_init_GT(test_val_2, pairing);

    // test_val_1 = e(peks_A, trapdoor_T) = e(g^r, H1(kw)^alpha) = e(g, H1(kw))^(r*alpha)
    pairing_apply(test_val_1, *peks_A, *trapdoor_T, pairing);

    // test_val_2 es simplemente peks_B, que es e(g^alpha, H1(kw))^r = e(g, H1(kw))^(alpha*r)
    element_set(test_val_2, *peks_B);

    // Comparar si los dos valores son iguales
    if (!element_cmp(test_val_1, test_val_2)) {
        printf("Prueba exitosa: La palabra clave COINCIDE.\n");
        element_clear(test_val_1);
        element_clear(test_val_2);
        return 1; // Coincide
    } else {
        printf("Prueba fallida: La palabra clave NO COINCIDE.\n");
        element_clear(test_val_1);
        element_clear(test_val_2);
        return 0; // No coincide
    }
}


int main(int argc, char **argv) {
    pairing_t pairing;
    pks_keys_t pks;
    sks_keys_t sks;

    // Usamos parámetros de curva tipo A (simétrica)
    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    
    // --- Flujo del programa ---
    printf("1. Generando claves...\n");
    keygen(pairing, &pks, &sks);
    
    printf("\n----------------------------------\n");
    
    // Palabra clave que queremos buscar
    char *keyword_to_search = "factura";
    
    printf("2. Generando el criptograma PEKS para '%s'...\n", keyword_to_search);
    element_t peks_A, peks_B;
    element_init_G1(peks_A, pairing);
    element_init_GT(peks_B, pairing);
    peks(&peks_A, &peks_B, keyword_to_search, &pks, pairing);
    
    printf("\n----------------------------------\n");
    
    printf("3. Generando la trampilla (trapdoor) para '%s'...\n", keyword_to_search);
    element_t trapdoor_T;
    element_init_G1(trapdoor_T, pairing);
    trapdoor(&trapdoor_T, keyword_to_search, &pks, &sks, pairing);
    
    printf("\n----------------------------------\n");
    
    printf("4. Realizando la prueba de coincidencia...\n");
    test(&peks_A, &peks_B, &trapdoor_T, pairing);

    printf("\n--- Probando con una palabra clave incorrecta ---\n");
    char *wrong_keyword = "confidencial";
    element_t wrong_trapdoor_T;
    element_init_G1(wrong_trapdoor_T, pairing);
    printf("Generando la trampilla (trapdoor) para '%s'...\n", wrong_keyword);
    trapdoor(&wrong_trapdoor_T, wrong_keyword, &pks, &sks, pairing);
    printf("Realizando la prueba...\n");
    test(&peks_A, &peks_B, &wrong_trapdoor_T, pairing);


    // Liberar memoria
    element_clear(pks.g);
    element_clear(pks.h);
    element_clear(pks.server_pk);
    element_clear(pks.user_pk);
    element_clear(sks.server_sk);
    element_clear(sks.user_sk);
    element_clear(peks_A);
    element_clear(peks_B);
    element_clear(trapdoor_T);
    element_clear(wrong_trapdoor_T);
    pairing_clear(pairing);

    return 0;
}
