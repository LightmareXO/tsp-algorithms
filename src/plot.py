# codex君に任せました

import argparse
import sys
from pathlib import Path

import matplotlib.pyplot as plt


def is_number(text):
    try:
        float(text)
    except ValueError:
        return False
    return True


def split_metadata(line):
    if "=" in line:
        return line.split("=", 1)
    if ":" in line:
        return line.split(":", 1)
    return None


def read_result(path):
    if path == "-":
        lines = [line.strip() for line in sys.stdin if line.strip()]
    else:
        with open(path, encoding="utf-8") as f:
            lines = [line.strip() for line in f if line.strip()]

    if not lines:
        raise ValueError("Result is empty.")

    metadata = {}
    score = ""
    path_points = []

    for line in lines:
        parts = line.split()

        if len(parts) >= 3:
            try:
                city = int(parts[0])
                x = float(parts[1])
                y = float(parts[2])
            except ValueError:
                pass
            else:
                path_points.append((city, x, y))
                continue

        metadata_pair = split_metadata(line)
        if metadata_pair:
            key, value = metadata_pair
            metadata[key.strip()] = value.strip()
            continue

        if not score and is_number(line):
            score = line
            continue

        if len(parts) < 3:
            continue

    if not path_points:
        raise ValueError("No path points found. Expected lines like: id x y")

    return score, path_points, metadata


def plot_route(result_path, output_path, testcase_name=None):
    score, path_points, metadata = read_result(result_path)

    xs = [x for _, x, _ in path_points]
    ys = [y for _, _, y in path_points]
    start_city, start_x, start_y = path_points[0]

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
                "color": "#2563eb",
                "lw": 1,
                "alpha": 0.65,
                "shrinkA": 3,
                "shrinkB": 3
            },
        )

    plt.scatter(xs, ys, s=30, zorder=3)
    plt.scatter(
        [start_x],
        [start_y],
        s=40,
        zorder=4,
    )

    for city, x, y in path_points:
        plt.annotate(
            str(city),
            (x, y),
            xytext=(2, 2),
            textcoords="offset points",
            fontsize=7,
        )

    title_parts = []
    testcase_title = testcase_name or metadata.get("NAME") or metadata.get("TESTCASE")
    if testcase_title:
        title_parts.append(Path(testcase_title).name)
    if "W" in metadata:
        title_parts.append(f"W={metadata['W']}")
    title_parts.append(f"score={score}")
    title_parts.append(f"s={start_city}")
    plt.title("  ".join(title_parts), fontsize=12)
    plt.axis("equal")
    plt.grid(True, alpha=0.25)
    ax.set_xticks([])
    ax.set_yticks([])
    ax.tick_params(labelsize=8)
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("result_file", help="C++ output file. Use '-' to read stdin.")
    parser.add_argument("-o", "--output", default="route.png")
    parser.add_argument("--testcase", default=None)
    args = parser.parse_args()

    plot_route(args.result_file, args.output, args.testcase)


if __name__ == "__main__":
    main()
