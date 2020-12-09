d3.chart = d3.chart || {};

/**
 * Dependency wheel chart for d3.js
 *
 * Usage:
 * var chart = d3.chart.dependencyWheel();
 * d3.select('#chart_placeholder')
 *   .datum({
 *      packageNames: [the name of the packages in the matrix],
 *      matrix: [your dependency matrix]
 *   })
 *   .call(chart);
 *
 * // Data must be a matrix of dependencies. The first item must be the main package.
 * // For instance, if the main package depends on packages A and B, and package A
 * // also depends on package B, you should build the data as follows:
 *
 * var data = {
 *   packageNames: ['Main', 'A', 'B'],
 *   matrix: [[0, 1, 1], // Main depends on A and B
 *            [0, 0, 1], // A depends on B
 *            [0, 0, 0]] // B doesn't depend on A or Main
 * };
 *
 * // You can customize the chart width, margin (used to display package names),
 * // and padding (separating groups in the wheel)
 * var chart = d3.chart.dependencyWheel().width(700).margin(150).padding(.02);
 *
 * @author François Zaninotto
 * @license MIT
 * @see https://github.com/fzaninotto/DependencyWheel for complete source and license
 */
d3.chart.dependencyWheel = function(options) {

    var width = 1000;
    var height = 1000;
    var margin = 150;
    var padding = 0.02;

    function chart(selection) {
        var baseSize;
        if (width < height)
        {
            baseSize = width;
        }
        else
        {
            baseSize = height;
        }

        selection.each(function(data) {

            var matrix = data.matrix;
            var packageNames = data.packageNames;
            var radius = baseSize / 2 - margin;

            // create the layout
            var chord = d3.layout.chord()
            .padding(padding)
            .sortSubgroups(d3.descending);


            // Select the svg element, if it exists.
            var svg = d3.select(this).selectAll("svg").data([data]);

            // Otherwise, create the skeletal chart.

            var gEnter = svg.enter().append("svg:svg")
            .attr("width", width + 2 * margin)
            .attr("height", height + 2 * margin)
            .attr("class", "dependencyWheel")
            .append("g")
            .attr("transform", "translate(" + width / 2 + "," + height/2 + ")");

            var arc = d3.svg.arc()
            .innerRadius(radius)
            .outerRadius(radius + 20);

            var fill = function(d) {
                if (d.index === 0) return '#ccc';
                var nameCode = packageNames[d.index][0].charCodeAt();
                return "hsl(" + parseInt(((  nameCode - 97) / 26) * 360, 10) + ",90%,70%)";
            };

            // Returns an event handler for fading a given chord group.
            var fade = function(opacity) {
                return function(g, i) {
                    svg.selectAll(".chord")
                    .filter(function(d) {
                        return d.source.index != i && d.target.index != i;
                    })
                    .transition()
                    .style("opacity", opacity);
                    var groups = [];
                    svg.selectAll(".chord")
                    .filter(function(d) {
                        if (d.source.index == i) {
                            groups.push(d.target.index);
                        }
                        if (d.target.index == i) {
                            groups.push(d.source.index);
                        }
                    });
                    groups.push(i);
                    var length = groups.length;
                    svg.selectAll('.group')
                    .filter(function(d) {
                        for (var i = 0; i < length; i++) {
                            if(groups[i] == d.index) return false;
                        }
                        return true;
                    })
                    .transition()
                    .style("opacity", opacity);
                };
            };

            chord.matrix(matrix);

            var rootGroup = chord.groups()[0];
            var rotation = - (rootGroup.endAngle - rootGroup.startAngle) / 2 * (180 / Math.PI);

            var g = gEnter.selectAll("g.group")
            .data(chord.groups)
            .enter().append("svg:g")
            .attr("class", "group")
            .attr("transform", function(d) {
                return "rotate(" + rotation + ")";
            });

            g.append("svg:path")
            .style("fill", fill)
            .style("stroke", fill)
            .attr("d", arc)
            .on("mouseover", fade(0.1))
            .on("mouseout", fade(1))
            .on('click', function (d) { itemClicked(packageNames[d.index]) } );

            g.append("svg:text")
            .each(function(d) { d.angle = (d.startAngle + d.endAngle) / 2; })
            .attr("dy", ".35em")
            .attr("text-anchor", function(d) { return d.angle > Math.PI ? "end" : null; })
            .attr("transform", function(d) {
                return "rotate(" + (d.angle * 180 / Math.PI - 90) + ")" +
                        "translate(" + (radius + 26) + ")" +
                        (d.angle > Math.PI ? "rotate(180)" : "");
            })
            .text(function(d) { return packageNames[d.index]; });

            gEnter.selectAll("path.chord")
            .data(chord.chords)
            .enter().append("svg:path")
            .attr("class", "chord")
            .style("stroke", function(d) { return d3.rgb(fill(d.source)).darker(); })
            .style("fill", function(d) { return fill(d.source); })
            .attr("d", d3.svg.chord().radius(radius))
            .attr("transform", function(d) {
                return "rotate(" + rotation + ")";
            })
            .style("opacity", 1);
        });
    }



    chart.width = function(value) {
        if (!arguments.length) return width;
        width = value;
        return chart;
    };
    chart.height = function(value) {
        if (!arguments.length) return height;
        height = value;
        return chart;
    };


    chart.margin = function(value) {
        if (!arguments.length) return margin;
        margin = value;
        return chart;
    };

    chart.padding = function(value) {
        if (!arguments.length) return padding;
        padding = value;
        return chart;
    };

    return chart;
};

d3.chart.updateDependencyWheel = function(options) {

    var width = 700;
    var height = 700;
    var margin = 100;
    var padding = 0.02;

    var gEnter = d3.select("svg")
    .attr("width", width + 2 * margin)
    .attr("height", height + 2 * margin)
    .attr("class", "dependencyWheel")
    .select("g")
    .attr("transform", "translate(" + width / 2 + "," + height/2 + ")");

    return;

    var matrix = data.matrix;
    var packageNames = data.packageNames;
    var radius = baseSize / 2 - margin;

    // create the layout
    var chord = d3.layout.chord()
    .padding(padding)
    .sortSubgroups(d3.descending);


    // Select the svg element, if it exists.
    var svg = d3.select(this).selectAll("svg").data([data]);

    // Otherwise, create the skeletal chart.
    var xTranslate;
    if (width < height)
    {
        //xTranslate = (width + 2 * margin) /2;
        xTranslate = width / 2;
    }
    else
    {
        //xTranslate = (width + 2 * margin) /2;
        xTranslate = width / 2;
    }
    var yTranslate;
    if (width < height)
    {
        yTranslate = height / 2;
        //yTranslate = (baseSize + 2 * margin) /2;
    }
    else
    {
        yTranslate = height / 2;
    }

    var gEnter = d3.select("svg")
    .attr("width", width + 2 * margin)
    .attr("height", height + 2 * margin)
    .attr("class", "dependencyWheel")
    .append("g")
    .attr("transform", "translate(" + width / 2 + "," + height/2 + ")");

    var arc = d3.svg.arc()
    .innerRadius(radius)
    .outerRadius(radius + 20);

    var fill = function(d) {
        if (d.index === 0) return '#ccc';
        var nameCode = packageNames[d.index][0].charCodeAt();
        return "hsl(" + parseInt(((  nameCode - 97) / 26) * 360, 10) + ",90%,70%)";
    };

    // Returns an event handler for fading a given chord group.
    var fade = function(opacity) {
        return function(g, i) {
            svg.selectAll(".chord")
            .filter(function(d) {
                return d.source.index != i && d.target.index != i;
            })
            .transition()
            .style("opacity", opacity);
            var groups = [];
            svg.selectAll(".chord")
            .filter(function(d) {
                if (d.source.index == i) {
                    groups.push(d.target.index);
                }
                if (d.target.index == i) {
                    groups.push(d.source.index);
                }
            });
            groups.push(i);
            var length = groups.length;
            svg.selectAll('.group')
            .filter(function(d) {
                for (var i = 0; i < length; i++) {
                    if(groups[i] == d.index) return false;
                }
                return true;
            })
            .transition()
            .style("opacity", opacity);
        };
    };

    chord.matrix(matrix);

    var rootGroup = chord.groups()[0];
    var rotation = - (rootGroup.endAngle - rootGroup.startAngle) / 2 * (180 / Math.PI);

    var g = gEnter.selectAll("g.group")
    .data(chord.groups)
    .enter().append("svg:g")
    .attr("class", "group")
    .attr("transform", function(d) {
        return "rotate(" + rotation + ")";
    });

    g.append("svg:path")
    .style("fill", fill)
    .style("stroke", fill)
    .attr("d", arc)
    .on("mouseover", fade(0.1))
    .on("mouseout", fade(1));

    g.append("svg:text")
    .each(function(d) { d.angle = (d.startAngle + d.endAngle) / 2; })
    .attr("dy", ".35em")
    .attr("text-anchor", function(d) { return d.angle > Math.PI ? "end" : null; })
    .attr("transform", function(d) {
        return "rotate(" + (d.angle * 180 / Math.PI - 90) + ")" +
                "translate(" + (radius + 26) + ")" +
                (d.angle > Math.PI ? "rotate(180)" : "");
    })
    .text(function(d) { return packageNames[d.index]; });

    gEnter.selectAll("path.chord")
    .data(chord.chords)
    .enter().append("svg:path")
    .attr("class", "chord")
    .style("stroke", function(d) { return d3.rgb(fill(d.source)).darker(); })
    .style("fill", function(d) { return fill(d.source); })
    .attr("d", d3.svg.chord().radius(radius))
    .attr("transform", function(d) {
        return "rotate(" + rotation + ")";
    })
    .style("opacity", 1);



    chart.width = function(value) {
        if (!arguments.length) return width;
        width = value;
        return chart;
    };
    chart.height = function(value) {
        if (!arguments.length) return height;
        height = value;
        return chart;
    };


    chart.margin = function(value) {
        if (!arguments.length) return margin;
        margin = value;
        return chart;
    };

    chart.padding = function(value) {
        if (!arguments.length) return padding;
        padding = value;
        return chart;
    };

    return chart;
};

var buildMatrixFromDependenciesJson = function(dependenciesjson) {
    //1. создаем общий список зависимостей
    var libraries = [];
    var targets = dependenciesjson;
    var dependencies = [];

    for (var key in targets)
    {
        if (libraries.indexOf(key) == -1)
        {

            libraries.push(key);
        }

        var libDependensies = targets[key];

        for (var k = 0; k < libDependensies.length; k++)
        {
            dependencies[key] = libDependensies[k];
            if (libraries.indexOf(libDependensies[k]) == -1)
            {

                libraries.push(libDependensies[k]);
            }
        }
    }

    //2. получаем значения

    var depmatrix = [];
    depmatrix[0] = [0];

    var increment = 0;

    for (var i = 0 ; i < libraries.length ; i++)
    {

        depmatrix[i] = [];
        depmatrix[i] = Array.apply(null, new Array(libraries.length)).map(Number.prototype.valueOf,0); //обнуляем

        depmatrix[i][i] = 0;//1 + increment;
        increment += 0.001;

        var libName = libraries[i]; //либа,

        for (var key2 in dependencies)
        {
            if (key2 === libName)
            {
                var depName = dependencies[key2];
                //depmatrix[i][ libraries.indexOf(dependencies[key2]) ] = 1;
                depmatrix[i][ libraries.indexOf(depName) ] = 1;
            }
        }

        //fill code
        for (var key in targets)
        {
            if (key === libName)
            {
                var libDependensies = targets[key];
                for (var k = 0; k < libDependensies.length; k++)
                {
                    depmatrix[i][ libraries.indexOf(libDependensies[k]) ] = 1;
                }
            }
            var libDependensies = targets[key];
        }
    }

    return {
        matrix: depmatrix,
        packageNames: libraries
    }
};
