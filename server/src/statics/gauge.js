// .gauge {
//     width: 100px;
//     height: 100px;
// }

// .gauge > * {
//     width: 100%;
//     height: 100%;
//     position: relative;
// }

// .gauge-background {
//     border: brown 10px solid;
//     border-radius: 100%;
//     clip-path: polygon(0 0, 100% 0, 100% 100%, 50% 50%, 0 100%);
// }

// export default 
class Gauge {
    constructor(htmlParams, gaugeParams) {
        this.element = {
            gauge: document.createElement('div'),
            background: document.body.children[0],
            forground: document.body.children[0],
            value: document.createElement('span')
        };

        gaugeParams = gaugeParams || {};

        this.gauge = {
            min: gaugeParams.min || 0,
            max: gaugeParams.max || 100,
            stroke: 0,
            opening: 0
        };

        htmlParams = htmlParams || {};
        htmlParams.anchor = htmlParams.anchor || document.body;
        htmlParams.size = htmlParams.size || 100;
        htmlParams.thickness = htmlParams.thickness || 10;
        htmlParams.color = htmlParams.color || 'red';
        htmlParams.backgroundColor = htmlParams.backgroundColor || 'black';
        htmlParams.showValue = htmlParams.showValue || true;
        htmlParams.fontSize = htmlParams.fontSize || 20;
        htmlParams.transition = htmlParams.transition || 1;
        htmlParams.opening = htmlParams.opening || 360;

        this.gauge.stroke = Math.PI * (100 - htmlParams.thickness);

        this.gauge.opening = htmlParams.opening / 360 * this.gauge.stroke;

        // transform="scale(-1,1),rotate(${90 - (360 - htmlParams.opening) / 2})"

        this.element.gauge.innerHTML = `
        <svg viewBox="0 0 100 100">
            <circle cx="50" cy="50" r="${50 -
                htmlParams.thickness / 2}" stroke="${
            htmlParams.backgroundColor
        }" stroke-width="${
            htmlParams.thickness
        }" fill="none" stroke-dasharray="${this.gauge.stroke} ${((360 -
            htmlParams.opening) /
            360) *
            this.gauge.stroke}"
        stroke-dashoffset="${this.gauge.stroke}"/>
            <circle style="transition: stroke-dasharray ${
                htmlParams.transition
            }s;" cx="${htmlParams.size / 2}" cy="${htmlParams.size /
            2}" r="${htmlParams.size / 2 - htmlParams.thickness / 2}" stroke="${
            htmlParams.color
        }" stroke-width="${
            htmlParams.thickness
        }" fill="none" stroke-dasharray="${this.gauge.stroke} 0"
                stroke-dashoffset="${this.gauge.stroke}" /></svg>
        `;

        this.element.background = this.element.gauge.children[0].children[0];
        this.element.forground = this.element.gauge.children[0].children[1];
        if (htmlParams.showValue) {
            this.element.value.style.display = 'table';
            this.element.value.style.position = 'relative';
            this.element.value.style.top = `calc(-50% - ${htmlParams.fontSize /
                2}px)`;
            this.element.value.style.marginLeft = 'auto';
            this.element.value.style.marginRight = 'auto';
            this.element.value.style.fontSize = `${htmlParams.fontSize}px`;
            this.element.gauge.appendChild(this.element.value);
        }

        this.element.gauge.style.width = `${htmlParams.size}px`;
        this.element.gauge.style.height = `${htmlParams.size}px`;
        this.element.gauge.className += ' gauge';

        this.element.gauge.style.transform = `scale(-1,1), rotate(${90 - (360 - htmlParams.opening) / 2})`;

        htmlParams.anchor.appendChild(this.element.gauge);

        this.updateValue(0);
    }

    updateValue(value) {
        if (value < this.gauge.min) value = this.gauge.min;
        if (value > this.gauge.max) value = this.gauge.max;
        if (this.element.value) this.element.value.innerHTML = value;
        this.element.forground.setAttribute(
            'stroke-dasharray',
            `${this.gauge.stroke} ${this.gauge.stroke -
                (value / (this.gauge.max - this.gauge.min)) *
                    this.gauge.opening}`
        );
    }
}
