# codex君に任せました

import argparse
import sys

import matplotlib.pyplot as plt


def read_result(path):
    if path == "-":
        lines = [line.strip() for line in sys.stdin if line.strip()]
    else:
        with open(path, encoding="utf-8") as f:
            lines = [line.strip() for line in f if line.strip()]

    if not lines:
        raise ValueError("Result is empty.")

    score = lines[0]
    path_points = []

    for line in lines[1:]:
        parts = line.split()
        if len(parts) < 3:
            continue

        city = int(parts[0])
        x = float(parts[1])
        y = float(parts[2])
        path_points.append((city, x, y))

    if not path_points:
        raise ValueError("No path points found. Expected lines like: id x y")

    return score, path_points


def plot_route(result_path, output_path):
    score, path_points = read_result(result_path)

    xs = [x for _, x, _ in path_points]
    ys = [y for _, _, y in path_points]

    plt.figure(figsize=(9, 7))
    ax = plt.gca()

    for i in range(len(path_points)):
        _, x1, y1 = path_points[i]
        _, x2, y2 = path_points[(i + 1) % len(path_points)]

        ax.annotate(
            "",
            xy=(x2, y2),
            xytext=(x1, y1),
            arrowprops={
                "arrowstyle": "->",
                "color": "#2f6fbb",
                "lw": 1.1,
                "alpha": 0.75,
                "shrinkA": 6,
                "shrinkB": 6,
                "mutation_scale": 9,
            },
        )

    plt.scatter(xs, ys, s=28, color="#d94f45", zorder=3)

    for city, x, y in path_points:
        plt.text(x, y, str(city), fontsize=7, ha="left", va="bottom")

    plt.title(f"score={score}")
    plt.axis("equal")
    plt.grid(True, alpha=0.25)
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("result_file", help="C++ output file. Use '-' to read stdin.")
    parser.add_argument("-o", "--output", default="route.png")
    args = parser.parse_args()

    plot_route(args.result_file, args.output)


if __name__ == "__main__":
    main()
