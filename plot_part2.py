#!/usr/bin/env python3
"""
CPSC 457 Assignment 3 - Plotting Script for Part 2
Generates plots for Second Chance (Clock) page replacement algorithm
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# Set plot style
plt.style.use('seaborn-v0_8-darkgrid')

def load_data():
    """Load CSV data for Second Chance algorithm experiments"""
    try:
        clock_n_df = pd.read_csv('results/data/clock_vary_n.csv')
        clock_m_df = pd.read_csv('results/data/clock_vary_m.csv')
        return clock_n_df, clock_m_df
    except FileNotFoundError as e:
        print(f"Error: {e}")
        print("Make sure to run the CLK algorithm first to generate CSV files.")
        sys.exit(1)

def plot_vary_n(clock_n_df):
    """
    Plot 1: Page Faults vs n (number of bits in reference register)
    Fixed parameters: m=10, frames=50
    """
    plt.figure(figsize=(12, 7))
    
    plt.plot(clock_n_df['n'], clock_n_df['PageFaults'], 
             marker='o', markersize=5, linewidth=2, 
             color='#9B59B6', linestyle='-')
    
    plt.xlabel('n (Number of Bits in Reference Register)', fontsize=14, fontweight='bold')
    plt.ylabel('Number of Page Faults', fontsize=14, fontweight='bold')
    plt.title('Second Chance: Page Faults vs Reference Register Size\n(m=10, Frames=50)', 
              fontsize=16, fontweight='bold', pad=20)
    plt.grid(True, alpha=0.3)
    plt.xticks(range(1, 33, 2))  # Show every other n value for readability
    plt.tight_layout()
    
    # Save plot
    plt.savefig('results/plots/plot4_clock_vary_n.png', dpi=300, bbox_inches='tight')
    print("✓ Generated: results/plots/plot4_clock_vary_n.png")
    plt.close()

def plot_vary_m(clock_m_df):
    """
    Plot 2: Page Faults vs m (interval between register shifts)
    Fixed parameters: n=8, frames=50
    """
    plt.figure(figsize=(12, 7))
    
    plt.plot(clock_m_df['m'], clock_m_df['PageFaults'], 
             marker='s', markersize=4, linewidth=2, 
             color='#16A085', linestyle='-')
    
    plt.xlabel('m (Interval Between Register Shifts)', fontsize=14, fontweight='bold')
    plt.ylabel('Number of Page Faults', fontsize=14, fontweight='bold')
    plt.title('Second Chance: Page Faults vs Shift Interval\n(n=8, Frames=50)', 
              fontsize=16, fontweight='bold', pad=20)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # Save plot
    plt.savefig('results/plots/plot5_clock_vary_m.png', dpi=300, bbox_inches='tight')
    print("✓ Generated: results/plots/plot5_clock_vary_m.png")
    plt.close()

def print_statistics(clock_n_df, clock_m_df):
    """Print statistics about the Second Chance algorithm behavior"""
    print("\n" + "=" * 60)
    print("Second Chance Algorithm Statistics")
    print("=" * 60)
    
    print("\nExperiment 1: Varying n (m=10, frames=50)")
    print(f"  n range: {clock_n_df['n'].min()} to {clock_n_df['n'].max()}")
    print(f"  Page faults range: {clock_n_df['PageFaults'].min()} to {clock_n_df['PageFaults'].max()}")
    print(f"  Average page faults: {clock_n_df['PageFaults'].mean():.2f}")
    print(f"  Best n value: {clock_n_df.loc[clock_n_df['PageFaults'].idxmin(), 'n']}")
    
    print("\nExperiment 2: Varying m (n=8, frames=50)")
    print(f"  m range: {clock_m_df['m'].min()} to {clock_m_df['m'].max()}")
    print(f"  Page faults range: {clock_m_df['PageFaults'].min()} to {clock_m_df['PageFaults'].max()}")
    print(f"  Average page faults: {clock_m_df['PageFaults'].mean():.2f}")
    print(f"  Best m value: {clock_m_df.loc[clock_m_df['PageFaults'].idxmin(), 'm']}")
    print()

def main():
    """Main function to generate all Part 2 plots"""
    print("=" * 60)
    print("CPSC 457 Assignment 3 - Part 2 Plotting")
    print("Generating plots for Second Chance (Clock) algorithm")
    print("=" * 60)
    print()
    
    # Create plots directory if it doesn't exist
    os.makedirs('results/plots', exist_ok=True)
    
    # Load data
    print("Loading data from CSV files...")
    clock_n_df, clock_m_df = load_data()
    print(f"✓ Loaded Clock (vary n) data: {len(clock_n_df)} entries")
    print(f"✓ Loaded Clock (vary m) data: {len(clock_m_df)} entries")
    print()
    
    # Generate plots
    print("Generating plots...")
    plot_vary_n(clock_n_df)
    plot_vary_m(clock_m_df)
    
    # Print statistics
    print_statistics(clock_n_df, clock_m_df)
    
    print("=" * 60)
    print("All Part 2 plots generated successfully!")
    print("=" * 60)
    print()
    print("Generated plots:")
    print("  4. results/plots/plot4_clock_vary_n.png")
    print("  5. results/plots/plot5_clock_vary_m.png")
    print()

if __name__ == "__main__":
    main()
