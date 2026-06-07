import numpy as np
import matplotlib.pyplot as plt

plt.rcParams["font.family"] = ["Noto Sans Mono"]

n = 512
hop = n // 4  # 75% overlap

# Janela sqrt Hann
window = np.sqrt(np.hanning(n))

# Quadros centrados em k
frames = [-2, -1, 0, 1, 2]
labels = {
    -2: r"$k-2$",
    -1: r"$k-1$",
     0: r"$k$",
     1: r"$k+1$",
     2: r"$k+2$"
}

# Gradiente arco-íris
colors = plt.cm.rainbow(np.linspace(0, 1, len(frames)))

fig, ax = plt.subplots(figsize=(12, 4))

for frame, color in zip(frames, colors):

    start = (frame + 2) * hop

    x = np.arange(start, start + n)

    ax.plot(
        x,
        window,
        linewidth=2.5,
        color=color,
        label=f"Quadro {labels[frame]}"
    )

# Referência do quadro central
s = 2 * hop

ticks = [
    s,
    s + hop,
    s + 2 * hop,
    s + 3 * hop,
    s + n
]

tick_labels = [
    r"$s$",
    r"$s + h$",
    r"$s + 2h$",
    r"$s + 3h$",
    r"$s + n$"
]

ax.set_xticks(ticks)
ax.set_xticklabels(tick_labels)

total_length = 4 * hop + n

ax.set_title("Sobreposição entre Quadros com Janela √Hann")
ax.set_xlabel("amostras")
ax.set_ylabel("amplitude")

ax.set_xlim(0, total_length)
ax.set_ylim(0, 1.05)

ax.set_axisbelow(True)

ax.spines[['left', 'top', 'right', 'bottom']].set_visible(False)

ax.minorticks_on()
ax.grid(True, which='both', color='#efefff')

ax.legend()

plt.tight_layout()
plt.show()