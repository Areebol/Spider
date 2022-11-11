import re
import json
import requests
import time

def get_one_page(url):
    headers = {
        'User-Agent':'Mozilla/5.0 '\
        '(Windows NT 10.0; Win64; x64) '\
        'AppleWebKit/537.36 (KHTML, like Gecko) '\
        'Chrome/107.0.0.0 Safari/537.36 Edg/107.0.1418.35'
    }
    response = requests.get(url,headers=headers)
    if response.status_code == 200:
        print(response.text)
        return response.text
    print("fail")
    return None

def parse_one_page(html):
    pattern = re.compile(
        '<em class="">(.*?)</em>'
        '.*?src="(.*?)"'
        '.*?<span class="title">(.*?)</span>'
        '.*?<p class="">\s*(.*?)<br>'
        '\s*(.*?)'
        '&nbsp;/&nbsp;(.*?)'
        '&nbsp;/&nbsp;(.*?)\n'
        ,re.S
    )
    items = re.findall(pattern,html)

    for item in items:
        yield {
            'Index':item[0],
            # 'Image':item[1],
            'Title':item[2].strip(),
            'Author':item[3],
            'Time':item[4],
            'Country':item[5],
            'Type':item[6]
        }

def write_to_file(content):
    with open('result.txt','a',encoding='utf-8') as f:
        f.write(json.dumps(content,ensure_ascii=False)+'\n')

def main(offset):
    url = 'https://movie.douban.com/top250?start='+str(offset)+'&filter='
    html = get_one_page(url)
    for item in parse_one_page(html):
        print(item)
        write_to_file(item)


if __name__ == '__main__':
    for i in range(10):
        main(offset=i*25)