import numpy as np


def convolve(f, g):
    """
    f:np.ndarray(int64)
    g:np.ndarray(int64)

    return h:np.ndarray,f*g
    """
    fft_len = 1
    while 2*fft_len < len(f)+len(g)-1:
        fft_len *= 2
    fft_len *= 2

    Ff = np.fft.rfft(f, fft_len)
    Fg = np.fft.rfft(g, fft_len)

    Fh = Ff*Fg

    h = np.fft.irfft(Fh, fft_len)

    h = np.rint(h).astype(np.int64)

    return h[:len(f)+len(g)-1]


def convolve_mod(f, g, p):
    """
    f=2**15*f1+f2
    g=2**15*g1+g2
    fg=2**30*f1g1+2**15(f1g2+f2g1)+f2g2
    a=f1g1
    b=f1g2+f2g1
    c=f2g2
    fg=a<<30+b<<15+c
    """
    f1, f2 = np.divmod(f, 1 << 15)
    g1, g2 = np.divmod(g, 1 << 15)

    a = convolve(f1, g1) % p
    c = convolve(f2, g2) % p
    b = (convolve(f1, g2) % p+convolve(f2, g1) % p) % p

    h = (a << 30)+(b << 15)+c
    return h % p
