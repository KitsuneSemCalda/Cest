#include "../cest.h"

// Exemplo de uma struct
typedef struct {
    int id;
    const char* username;
    double level;
} User;

// Helper para asserção de igualdade customizada (Deep Equality)
static bool UsersAreEqual(User u1, User u2) {
    if (u1.id != u2.id) return false;
    if (u1.level != u2.level) return false;
    if (strcmp(u1.username, u2.username) != 0) return false;
    return true;
}

int main() {
    User u1 = {1, "kitsune", 42.5};
    User u2 = {1, "kitsune", 42.5};
    User u3 = {2, "tsoding", 99.9};

    describe("Struct Testing", {
        it("should check fields manually", {
            expect(u1.id).toEqual(1);
            expect(u1.username).toEqual("kitsune");
        });

        it("should support pointer equality checks", {
            User* ptr1 = &u1;
            User* ptr2 = &u1;
            expect(ptr1).toEqual(ptr2); // Mesmo endereço de memória
        });

        it("should compare structs using a custom checker function", {
            // O Cest não faz memcmp genérico para structs passadas por valor no expect(), 
            // então usamos toBeTruthy sobre uma função que retorna bool
            expect(UsersAreEqual(u1, u2)).toBeTruthy();
            expect(UsersAreEqual(u1, u3)).toBeFalsy();
        });
    });

    return cest_result();
}
