clear

<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
function OLLAMA_SETUP()
{
    ##.\OllamaSetup.exe /DIR="D:\z2025_1\AI\Ollama060"

    ollama.exe --version # 0.6.1 

    ## C:\Users\zvl_2\.ollama\models ?? 
    ollama pull deepseek-r1:7b
    ollama pull qwen2.5:14b-instruct-q4_K_M

    echo "FROM deepseek-r1:7b" > Deepseek-r1-7b-8k.txt
    echo "PARAMETER num_ctx 8096" >> Deepseek-r1-7b-8k.txt 
    ollama create deepseek-r1:7b-8k --file Deepseek-r1-7b-8k.txt

    echo "FROM qwen2.5:14b-instruct-q4_K_M" > Qwen-14b-Instruct-8k.txt 
    echo "PARAMETER num_ctx 8096" >> Qwen-14b-Instruct-8k.txt 
    ollama create qwen2.5:14b-instruct-8k --file Qwen-14b-Instruct-8k.txt 

    ollama list 
}


function ENV_SETUP() 
{
    python.exe --version 
    python.exe -m venv $ENV_NAME 
}


function PIP_SETUP() 
{
    try{ deactivate } catch{ }

    & $ENV 

    & $PYTHON_PATH --version 

    ##Invoke-WebRequest https://bootstrap.pypa.io/get-pip.py  -OutFile get-pip.py
    ##& $PYTHON_PATH get-pip.py 

    & $PYTHON_PATH -m pip install --upgrade pip
    & $PYTHON_PATH -m pip --version 

    & $PYTHON_PATH -m pip install pypdf 
    & $PYTHON_PATH -m pip install chromadb

    & $PYTHON_PATH -m pip install langchain
    & $PYTHON_PATH -m pip install langchain-chroma
    & $PYTHON_PATH -m pip install langchain-community
    & $PYTHON_PATH -m pip install langchain-huggingface

    & $PYTHON_PATH -m pip install smolagents 
    & $PYTHON_PATH -m pip install smolagents[openai]
    & $PYTHON_PATH -m pip install smolagents[gradio]
    # & $PYTHON_PATH -m 

    #& $PYTHON_PATH -m pip install -r requirements.txt
    & $PYTHON_PATH ingest_pdfs.py

    ## Test : 'Compare and contrast the services offered by RankBoost and Omni Marketing'
    & $PYTHON_PATH r1_smolagent_rag.py
}




function PIP_RUNNER() 
{

    & $PYTHON_PATH --version 

    streamlit.exe --version    

    streamlit.exe run Sources\app_2.py
}



<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
#$PYTHON_PATH="D:\z2025_1\AI\Python3140\python.exe"
$ROOT_PATH="D:\z2025_1\AI\RAGs\R1DistillRag1a1"

$ENV_NAME="Env4"
$ENV="${ROOT_PATH}\..\${ENV_NAME}\Scripts\Activate.ps1"

$PYTHON_PATH="python.exe"

#OLLAMA_SETUP 
#ENV_SETUP
PIP_SETUP 

#PIP_RUNNER


<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
<#
https://www.python.org/downloads/windows/

#>
<#---------------------------------------------------------------------------#>