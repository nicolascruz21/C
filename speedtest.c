#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função para realizar uma requisição HTTP GET para medir a velocidade de download
size_t download_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

double measure_download_speed()
{
    CURL *curl;
    CURLcode res;
    double download_speed = 0.0;

    curl = curl_easy_init();
    if (curl)
    {
        // URL de exemplo para download do arquivo
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/file-to-download");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_callback);

        // Inicia a contagem de tempo
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Realiza a requisição e mede a velocidade de download
        res = curl_easy_perform(curl);

        // Finaliza a contagem de tempo
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (res == CURLE_OK)
        {
            // Calcula a velocidade de download em Mbps
            double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            download_speed = (curl_off_t)size / total_time / 1024 / 1024;
        }

        curl_easy_cleanup(curl);
    }

    return download_speed;
}

// Função para medir o tempo de ping
double measure_ping()
{
    double ping_time = 0.0;

    // Comando de exemplo para realizar o ping
    FILE *ping_output = popen("ping -c 1 example.com", "r");
    if (ping_output)
    {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), ping_output) != NULL)
        {
            // Procura pela linha que contém o tempo de ping
            char *ping_str = strstr(buffer, "time=");
            if (ping_str)
            {
                // Extrai o valor numérico do tempo de ping
                double ping_value = atof(ping_str + 5);
                ping_time = ping_value;
                break;
            }
        }

        pclose(ping_output);
    }

    return ping_time;
}

int main()
{
    // Realiza uma medição de velocidade de download
    double download_speed = measure_download_speed();
    printf("Velocidade de Download: %.2f Mbps\n", download_speed);

    // Realiza uma medição de velocidade de upload (envio de um arquivo vazio)
    CURL *curl = curl_easy_init();
    if (curl)
    {
        // URL de exemplo para upload do arquivo
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/upload.php");
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE, 0L);

        // Inicia a contagem de tempo
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Realiza a requisição e mede a velocidade de upload
        curl_easy_perform(curl
