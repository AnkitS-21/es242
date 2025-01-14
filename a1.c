#include "test.h"

#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */
void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
int index = 0;

    int selected[k];

    void generate(int start) {
        if (index == k) {

            process_selection(selected, k, data);
            return;
        }

        for (int i = start; i < n; ++i) {
            selected[index++] = a[i];
            generate(i + 1);
            index--;
        }
    }

    generate(0);
}

/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */
void generate_splits_recursive(const char *input_str, const char *word_list[], int num_words, char output_buf[], int input_index, int output_index, void *custom_data, void (*split_processor)(char output_buf[], void *custom_data))
{
    if (input_index == strlen(input_str)) {
        output_buf[output_index] = '\0';
        split_processor(output_buf, custom_data);
        return;
    }
    char current_word[100];
    int current_word_index = 0;
    while (input_index < strlen(input_str)) {
        current_word[current_word_index] = input_str[input_index];
        current_word_index++;
        input_index++;
        current_word[current_word_index] = '\0';

        int word_idx = 0;
        while (word_idx < num_words) {
            if (strcmp(current_word, word_list[word_idx]) == 0) {
                strcpy(output_buf + output_index, current_word);
                output_index += strlen(current_word);
                if (input_index < strlen(input_str)) {
                    output_buf[output_index] = ' ';
                    output_index++;
                }
                generate_splits_recursive(input_str, word_list, num_words, output_buf, input_index, output_index, custom_data, split_processor);

                output_index -= strlen(current_word);
                if (input_index < strlen(input_str)) {
                    output_index--;
                }
            }
            word_idx++;
        }
    }
}

void generate_splits(const char *input_str, const char *word_list[], int num_words, char output_buf[], void *custom_data, void (*split_processor)(char output_buf[], void *custom_data))
{
    int input_index = 0;
    int output_index = 0;
    generate_splits_recursive(input_str, word_list, num_words, output_buf, input_index, output_index, custom_data, split_processor);
}
/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
void previous_permutation(int a[], int n)
{
    // Find the largest index i such that a[i] > a[i+1]
    int i = n - 2;
    while (i >= 0 && a[i] <= a[i + 1]) {
        i--;
    }

    // If there is no such index, it means the array is the first permutation, so leave it unchanged
    if (i == -1) {
        return;
    }

    // Find the largest index j such that a[j] < a[i]
    int j = n - 1;
    while (a[j] >= a[i]) {
        j--;
    }

    // Swap a[i] and a[j]
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;

    // Reverse the elements from a[i+1] to the end of the array
    int left = i + 1;
    int right = n - 1;
    while (left < right) {
        int temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        left++;
        right--;
        
    }
}

/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
    int first;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

void count_selections(int b[], int k, void *data)
{
    int *d = (int*)data;
    ++*d;
}

typedef struct {
    int b[100];
} selection_t;

void last_selection(int b[], int k, void *data)
{
    selection_t *s = (selection_t*)data;
    for (int i = 0; i < k; ++i) {
        s->b[i] = b[i];
    }
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[10];
    state_t s2165 = { .index = 0, .err = 1, .first = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
    int c;
    int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(aa, 10, 5, b, &c, count_selections);
    ASSERT_EQ(c, 288, "Failed on 10C5.");

    selection_t s;
    generate_selections(aa, 10, 5, b, &s, last_selection);
    ASSERT_ARRAY_VALUES_EQ(s.b, 5, "Failed on last of 10C5.", 12, 4, 3, 6, 6);
} END_TEST

void test_splits_art(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if (strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1, .first = 1 };
    char buf[256];
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
    int aa[] = { 1, 2, 3, 5, 4, 6 };
    previous_permutation(aa, 3); // 3 is correct.
    ASSERT_ARRAY_VALUES_EQ(aa, 3, "Failed on 1 2 3.", 1, 2, 3);
} END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
        });
    return 0;
}
