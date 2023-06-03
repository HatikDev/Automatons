from PIL import Image, ImageDraw
from collections import deque
import math

class Node:
    def __init__(self, id, parent, left, right, value):
        self.id = id
        self.parent = parent
        self.left = left
        self.right = right
        self.value = value


def read_nodes(filename):
    file = open(filename)
    nodes = dict()
    params = list()
    is_first_node = True
    for line in file.readlines():
        params = line.split()
        for param in params:
            key, value = map(str, param.split(":"))
            # match key:
            #     case "id": node_id = value
            #     case "parent": node_parent = value
            #     case "left": node_left = value
            #     case "right": node_right = value
            #     case "value": node_value = value
            if key == "id":
                node_id = value
            if key == "parent":
                node_parent = value
            if key == "left":
                node_left = value
            if key == "right":
                node_right = value
            if key == "value":
                node_value = value
        if is_first_node:
            nodes["root"] = node_id
        nodes[node_id] = Node(node_id, node_parent, node_left, node_right, node_value)
        is_first_node = False
    file.close()
    return nodes


def _draw_ellipse(draw, x0, y0, value): # TODO: remove all magic values
    
    draw.ellipse([x0 - 25, y0 - 25, x0 + 25, y0 + 25], fill=(44, 27, 133, 0))
    draw.text((x0 - 20, y0 - 10), value, (150, 150, 150))


def _draw_relative_links(draw, pos1, pos2):
    x0, y0 = pos1
    x1, y1 = pos2
    draw.line()


def print_tree(nodes, draw):
    level = 0
    old_level = -1
    offset_in_level = 1
    current_nodes = deque([(nodes["root"], 0)])
    while len(current_nodes):
        current_node_id, level = current_nodes.popleft()
        if level == old_level:
            offset_in_level += 1
        else:
            offset_in_level = 1
        value = nodes[current_node_id].value

        x0 = width / (2 ** level + 1) * offset_in_level
        y0 = 30 + level * 100
        _draw_ellipse(draw, x0, y0, value)
        # _draw_ellipse(draw, width, value, level, offset_in_level)
        # _draw_relative_links(draw, pos1, pos2)

        left = nodes[current_node_id].left
        right = nodes[current_node_id].right
        if int(left, 16) > 0:
            current_nodes.extend([(left, level + 1)])
        if int(right, 16) > 0:
            current_nodes.extend([(right, level + 1)])
        old_level = level


width = 1440
height = 900


img  = Image.new(mode = "RGB", size = (width, height), color = (255, 255, 255))
draw = ImageDraw.Draw(img)
print_tree(read_nodes("ast.txt"), draw)
img.show()
