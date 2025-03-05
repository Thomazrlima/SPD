from difflib import SequenceMatcher

with open("C.txt") as f1, open("C1.txt") as f2:
    conteudo1 = f1.read()
    conteudo2 = f2.read()

similaridade = SequenceMatcher(None, conteudo1, conteudo2).ratio() * 100
print(f"Similaridade: {similaridade:.2f}%")
