import xml.etree.ElementTree as ET
from graphviz import Digraph

def main():
    # Load the XML file
    tree = ET.parse('/home/asif/XML generation/output.xml')  # Update this path
    root = tree.getroot()

    # Namespace map
    ns = {'sspaceex': 'http://www-verimag.imag.fr/xml-namespaces/sspaceex'}

    # Create a Digraph for the visualization
    dot = Digraph(comment='Warehouse Automation Agent')

    # Extract and add locations as nodes, taking namespaces into account
    for location in root.findall('.//sspaceex:location', ns):
        loc_id = location.get('id')
        loc_name = location.get('name')
        dot.node(loc_id, loc_name)

    # Extract and add transitions as edges, taking namespaces into account
    for transition in root.findall('.//sspaceex:transition', ns):
        source = transition.get('source')
        target = transition.get('target')
        label = transition.find('.//sspaceex:label', ns).text if transition.find('.//sspaceex:label', ns) is not None else ""
        dot.edge(source, target, label=label)

    # Render the graph to a file (e.g., in PNG format)
    dot.render('warehouse_automation_agent_diagram', format='png', view=True)

if __name__ == '__main__':
    main()


""" import xml.etree.ElementTree as ET
from graphviz import Digraph

def main():
    tree = ET.parse('/home/asif/XML generation/output.xml')  # Update this path
    root = tree.getroot()

    ns = {'sspaceex': 'http://www-verimag.imag.fr/xml-namespaces/sspaceex'}
    dot = Digraph(comment='Warehouse Automation Agent', engine='dot')
    dot.attr(rankdir='BT')  # Bottom to Top layout

    # Assuming IDs are sequential and start from 1, adjust as necessary
    locations = root.findall('.//sspaceex:location', ns)
    
    # Sort locations by id to ensure sequential arrangement
    locations = sorted(locations, key=lambda x: int(x.get('id')))

    max_per_row = 4  # Max locations per row, adjust based on your grid needs
    current_row = 0
    for index, location in enumerate(locations):
        loc_id = location.get('id')
        loc_name = location.get('name')

        # Determine row and position in row based on index
        if index % max_per_row == 0:
            current_row += 1
        position_in_row = index % max_per_row

        # Add node with position hint in label (you might omit this in final version)
        dot.node(loc_id, f'{loc_name}\n(Pos: {position_in_row}, Row: {current_row})')

        # Add invisible edges to enforce layout
        if position_in_row > 0:  # Not the first node in a row
            # Horizontal alignment (left to right)
            dot.edge(str(index), loc_id, style="invis")
        if index >= max_per_row:  # Not in the first row
            # Vertical alignment (bottom to top), connect to node directly below in the next row
            below_id = str(index - (max_per_row - 1))
            dot.edge(below_id, loc_id, style="invis")

    for transition in root.findall('.//sspaceex:transition', ns):
        source = transition.get('source')
        target = transition.get('target')
        label = transition.find('.//sspaceex:label', ns).text if transition.find('.//sspaceex:label', ns) is not None else ""
        # Add actual edges for transitions
        dot.edge(source, target, label=label)

    dot.render('warehouse_automation_agent_diagram', format='png', view=True)

if __name__ == '__main__':
    main()
 """